import random

# This file wasn't made by a human

names = ["foo", "bar", "baz", "qux", "x", "y", "z", "a", "b", "c", "result", "temp", "val", "num", "count"]
ops = ["+", "-", "*", "/", "&", "|", "^", ">>", "<<"]
assign_ops = ["+=", "-=", "*=", "/="]
strings = ['"hello"', '"world"', '"test"', '"foo %d\\n"', '"result: %d\\n"']

lines = []

def rand_name():
    return random.choice(names)

def rand_expr():
    a, b = rand_name(), rand_name()
    op = random.choice(ops)
    return f"{a} {op} {b}"

def rand_stmt(indent):
    t = random.randint(0, 5)
    if t == 0:
        return f"{indent}var {rand_name()} = {random.randint(0, 1000)}"
    elif t == 1:
        return f"{indent}let {rand_name()} = {rand_expr()}"
    elif t == 2:
        return f"{indent}{rand_name()} = {rand_expr()}"
    elif t == 3:
        op = random.choice(assign_ops)
        return f"{indent}{rand_name()} {op} {random.randint(1, 100)}"
    elif t == 4:
        return f"{indent}printf({random.choice(strings)}, {rand_name()})"
    else:
        return f"{indent}# {random.choice(['compute', 'assign', 'update', 'check', 'result'])}"

def gen_func():
    name = rand_name() + str(random.randint(0, 999))
    params = ", ".join(random.sample(names, random.randint(0, 3)))
    body = []
    for _ in range(random.randint(3, 12)):
        body.append(rand_stmt("\t"))
    body.append(f"\treturn {rand_expr()}")
    return "\n".join([f"func {name}({params}) {{"] + body + ["}"])

def gen_obj():
    name = rand_name().capitalize() + str(random.randint(0, 99))
    methods = []
    for _ in range(random.randint(1, 3)):
        mname = rand_name()
        body = [rand_stmt("\t\t") for _ in range(random.randint(2, 5))]
        methods.append(f"\tfunc {mname}() {{\n" + "\n".join(body) + "\n\t}}")
    return f"obj {name}() {{\n" + "\n".join(methods) + "\n}"

# header
lines.append("# require <stdio>")
lines.append("")
lines.append(f"var g = 0")
lines.append("")

while len(lines) < 200000:
    t = random.randint(0, 3)
    if t == 0:
        lines.append(gen_func())
    elif t == 1:
        lines.append(gen_obj())
    elif t == 2:
        lines.append(f"var {rand_name()}{random.randint(0,999)} = {random.randint(0, 9999)}")
    else:
        lines.append(f"let {rand_name()}{random.randint(0,999)} = {rand_expr()}")
    lines.append("")

with open("./tests/200k.t", "w") as f:
    f.write("\n".join(lines))

import subprocess
result = subprocess.run(["wc", "-l", "./tests/200k.t"], capture_output=True, text=True)
print(result.stdout.strip())