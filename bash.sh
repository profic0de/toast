#!/usr/bin/env bash

cat <<EOF > main.haste
const Vec2 = struct {
    x: auto = 6.0;
    y: auto = 7.0;
};

// same as above
const Point = struct {
    x := 6.0;
    y := 7.0;
};

const i: float = 6;
const a: float = 7;
const b: float = 6;
const c: float = 7;

const a0: Vec2 = .{};
const b0: Point = .{};
EOF

i=1
while (( i < 200000 )); do
    echo "const a$i: Vec2 = .{ x: i + a / b * c, y: i + a / b * c };" >> main.haste 
    i=$(( i + 1 ))
done