import os

def to_base(n, b):
    if n == 0: return "0"
    res = ""
    while n:
        res = str(n % b) + res
        n //= b
    return res

os.makedirs("expected", exist_ok=True)

for file in os.listdir("tests"):
    with open(f"tests/{file}") as f:
        I1, I2, B = f.read().split()
        B = int(B)

    a = int(I1, B)
    b = int(I2, B)

    out = f"{to_base(a+b,B)} {to_base(a*b,B)} {to_base(a//b if b!=0 else 0,B)}"

    with open(f"expected/{file}", "w") as f:
        f.write(out)