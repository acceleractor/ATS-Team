import base64

food_seller_said = b"ZmxhZ3tFdmVyeV9BcHBsZV9Db3N0X1RoaXJ0eV9jb2lufQ=="

print("the food_seller said something to you, try to translate it ,you only have one chance,there is no time left!!!!!!")

flag = input()
flag = bytes(flag,encoding = "utf8")
flag = base64.b64encode(flag)

for i in flag:
    i^=32


if flag == food_seller_said:
    print("congratulation, you have enhanced what the food_seller said, you can avoiod being starved")
else:
    print("oops,it seems you are going to starve here")
