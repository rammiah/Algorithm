import re

pat = re.compile(r'[a-zA-Z]+')
with open('test.txt', 'r', encoding='utf-8') as fp:
    lines = fp.readlines()
result = []
for line in lines:
    line = line.strip()
    words = pat.findall(line)
    if len(words) > 0:
        result.append(' '.join(words) + '\n')
with open('text.txt', 'w', encoding='utf-8') as fp:
    fp.writelines(result)
