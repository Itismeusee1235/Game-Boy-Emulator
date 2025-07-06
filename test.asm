SECTION "Main", ROM0[$0100]
    ld a, $48
    ldh [0x01], a     ; write to 0xFF01
    ld a, $81
    ldh [0x02], a     ; write to 0xFF02 — triggers output
    jp $0100
