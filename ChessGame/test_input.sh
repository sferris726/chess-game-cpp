#!/usr/bin/expect

spawn ./build/ChessGame

set moves {
        "g1 f3" \
        "b8 c6" \
        "b1 c3" \
        "g8 f6" \
        "c3 b5" \
        "f6 g4" \
        "h2 h3" \
        "a7 a6" \
        "b5 a7" \
        "g4 h2" \
        "f3 h2" \
        "c6 a7" \
        "g2 g4" \
        "b7 b5" \
        "f1 g2" \
        "c8 b7" \
        "e2 e4" \
        "d7 d5" \
        "e1 e2" \
        "e8 d7" \
        "d1 g1" \
        "d8 b8" \
        "b2 b4" \
        "g7 g5" \
        "c1 b2" \
        "f8 g7" \
        "a1 f1" \
        "h8 c8" \
        "b2 d4" \
        "g7 e5" \
        "f2 f3" \
        "c7 c6" \
        "d4 f2" \
        "e5 c7" \
        "f2 e1" \
        "c7 d8" \
        "e2 f2" \
        "d7 c7" \
        "a2 a4" \
        "h7 h5" \
        "a4 a5" \
        "h5 h4" \
        "c2 c4" \
        "f7 f5" \
        "c4 c5" \
        "f5 f4" \
        "e4 e5" \
        "d5 d4" \
        "e5 e6" \
        "d4 d3" \
    }

for {set i 0} {$i < [llength $moves]} {incr i} {
    # Expect the prompt for the current turn
    if {($i % 2) == 0} {
        expect "White turn, please enter move: "
        send "[lindex $moves $i]\r"
    } else {
        expect "Black turn, please enter move: "
        send "[lindex $moves $i]\r"
    }

    sleep 0.2
}

expect eof