chip = 'WQZT`blf\\UZgV\\hfXXVhhWfQQb\\[kRoT\\[ZXP\\iUV\\hVXkVi~'
flag = ''

for char in chip:
    original_char = chr((ord(char) ^ 0xE) + 13)
    flag += original_char

print(flag)