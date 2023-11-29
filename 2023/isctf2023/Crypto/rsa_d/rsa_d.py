import gmpy2
# input your own number
p=14785247
q=16612481
e=65537
#n = p * q

# solve part
phi_n = (p - 1) * (q - 1)
d =gmpy2.invert(e, phi_n)
print(d)