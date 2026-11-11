# CL Toolkit pre-flight checker. ЗАПУСКАТЬ ПОСЛЕ ЛЮБЫХ правок файлов мода:
#   python _mod_tools\clt_check.py
# Ловит классы ошибок, которые уже ломали сегмент (окно не открывается по Insert):
#   1) несбалансированные скобки по всему сегменту (include-цепочка как один файл)
#   2) незакрытые строковые литералы и НЕподдерживаемые escape \ внутри литералов
#   3) неизвестные символы мода (CLT_*/clt_* без определения) — как CLT_SEC_MISC
#   4) #include несуществующих файлов — как clt_ach_table.c
#   5) не-UTF8, контрольные символы в литералах, длинные литералы, число аргументов CLT_Ach_Add
#   6) перенос условия if/while/for на следующую строку (Storm: «missed ')'»)
# Exit code: 0 = PASS, 1 = FAIL.
import os, re, sys

GAME = r'C:\Games\Steam\steamapps\common\Caribbean Legend'
MOD = os.path.join(GAME, 'mod_cltoolkit', 'Program')
SEG = 'cltoolkit\\clt_window.c'
MAXLIT = 400

errors = []


def err(f, ln, msg):
    errors.append('%s(%d): %s' % (f, ln, msg))


def resolve(x):
    for base in (MOD, os.path.join(GAME, 'Program')):
        p = os.path.join(base, x)
        if os.path.isfile(p):
            return p
    return None


def process_file(path, segname, seen, depth, lines_out):
    key = segname.lower()
    if key in seen:
        return
    seen[key] = True
    try:
        raw = open(path, 'rb').read()
        text = raw.decode('utf-8', 'strict')
    except UnicodeDecodeError as e:
        err(segname, 0, 'NOT valid UTF-8: %s' % e)
        return
    if raw.count(b'\n') - raw.count(b'\r\n') > 0:
        err(segname, 0, 'bare LF line endings (expected CRLF)')

    in_str = False
    lit_len = 0
    block_comment = False
    for ln, line in enumerate(text.splitlines(), 1):
        m = re.match(r'\s*#include\s+"(.+?)"', line)
        if m:
            tgt = m.group(1)
            rp = resolve(tgt)
            if rp is None:
                err(segname, ln, '#include target not found: %s' % tgt)
            else:
                process_file(rp, tgt, seen, depth, lines_out)
            continue
        code = []
        i = 0
        n = len(line)
        while i < n:
            ch = line[i]
            if block_comment:
                if ch == '*' and i + 1 < n and line[i + 1] == '/':
                    block_comment = False
                    i += 2
                    continue
                i += 1
                continue
            if in_str:
                # backslash в литерале — ЛИТЕРАЛЬНЫЙ символ (движок не обрабатывает escape);
                # ошибка — только если он вне строки (случается после раннего закрытия кавычкой)
                if ch == '"':
                    in_str = False
                    if lit_len > MAXLIT:
                        err(segname, ln, 'string literal too long: %d chars (max %d)' % (lit_len, MAXLIT))
                    i += 1
                    continue
                lit_len += 1
                if ord(ch) < 32:
                    err(segname, ln, 'control char U+%04X inside string literal' % ord(ch))
                i += 1
                continue
            if ch == '"':
                in_str = True
                lit_len = 0
                i += 1
                continue
            if ch == '\\':
                err(segname, ln, 'bare \\ outside string literal (раннее закрытие строки кавычкой?)')
                i += 1
                continue
            if ch == '/' and i + 1 < n and line[i + 1] == '/':
                break
            if ch == '/' and i + 1 < n and line[i + 1] == '*':
                block_comment = True
                i += 2
                continue
            code.append(ch)
            if ch == '{':
                depth[0] += 1
            elif ch == '}':
                depth[0] -= 1
                if depth[0] < 0:
                    err(segname, ln, 'unbalanced } (extra closing brace)')
                    depth[0] = 0
            i += 1
        if in_str:
            err(segname, ln, 'unterminated string literal at end of line')
            in_str = False
        # Storm Engine не поддерживает перенос условия if/while/for на следующую строку
        # (иначе «missed ')'» при компиляции). Условие должно помещаться на одной строке.
        if re.search(r'\b(if|while|for)\s*\(', line) and (''.join(code).count('(') - ''.join(code).count(')')) > 0:
            err(segname, ln, 'multi-line if/while/for condition (Storm requires single line; missed \')\')')
        lines_out.append((segname, ln, ''.join(code)))
    if in_str:
        err(segname, 0, 'unterminated string at EOF')


def main():
    seg_path = resolve(SEG)
    if seg_path is None:
        print('FATAL: segment not found:', SEG)
        return 1
    seen = {}
    depth = [0]
    lines_out = []
    process_file(seg_path, SEG, seen, depth, lines_out)
    if depth[0] != 0:
        err('(segment)', 0, 'brace depth at EOF = %d (missing %s)'
            % (depth[0], '}' * depth[0] if depth[0] > 0 else '{' * (-depth[0])))

    # --- символы мода: определены vs использованы (регистр не важен) ---
    # определения: сегмент окна + его include-цепочка + mod_inits (другой сегмент мода,
    # его функции вызываются кросс-сегментно, как CLT_FinishClose)
    defined = set()
    def collect_defs(text):
        for code_line in text.splitlines():
            for m in re.finditer(r'^\s*#define\s+(\w+)', code_line):
                defined.add(m.group(1).lower())
            for m in re.finditer(r'^\s*(?:void|string|int|float|bool|ref|aref|object)\s+(\w+)\s*\(', code_line):
                defined.add(m.group(1).lower())
            for m in re.finditer(r'^\s*(?:string|int|bool|float|ref|aref|object)\s+(\w+)\s*[;\[=]', code_line):
                defined.add(m.group(1).lower())
    for segname, ln, code in lines_out:
        collect_defs(code)
    inits_path = os.path.join(MOD, 'mod_inits', 'cltoolkit.c')
    if os.path.isfile(inits_path):
        collect_defs(open(inits_path, encoding='utf-8').read())
    for segname, ln, code in lines_out:
        for m in re.finditer(r'(?<![\w.])\b([Cc][Ll][Tt]_\w+)', code):
            if m.group(1).lower() not in defined:
                err(segname, ln, 'unknown symbol %s' % m.group(1))

    # --- CLT_Ach_Add: ровно 5 аргументов ---
    for segname, ln, code in lines_out:
        p = code.find('CLT_Ach_Add(')
        if p < 0:
            continue
        p += len('CLT_Ach_Add(')
        args = 1
        d = 0
        instr = False
        while p < len(code):
            ch = code[p]
            if ch == '"':
                instr = not instr
            elif not instr:
                if ch in '({':
                    d += 1
                elif ch in ')}':
                    if d == 0:
                        break
                    d -= 1
                elif ch == ',' and d == 0:
                    args += 1
            p += 1
        if args != 5:
            err(segname, ln, 'CLT_Ach_Add has %d args (expected 5)' % args)

    if errors:
        print('FAIL — %d problem(s):' % len(errors))
        for e in errors:
            print('  ' + e)
        return 1
    print('PASS: %d file(s), %d lines, braces OK, symbols OK, literals OK' % (len(seen), len(lines_out)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
