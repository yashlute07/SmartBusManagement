default rel

extern printf
extern scanf

global main

section .data

title db 10, "===== SMART BUS FARE CALCULATION =====", 10, 0

menu db 10
     db "1. Pune   - Rs. 20", 10
     db "2. Nashik - Rs. 50", 10
     db "3. Mumbai - Rs. 80", 10
     db "4. Exit", 10
     db "Enter destination choice: ", 0

inputFormat db "%d", 0
fareFormat db "Fare: Rs. %d", 10, 0
invalidMsg db "Invalid choice.", 10, 0

section .bss
choice resd 1

section .text

main:

    ; Windows x64 shadow space
    sub rsp, 40

    lea rcx, [title]
    call printf

menu_loop:

    lea rcx, [menu]
    call printf

    lea rcx, [inputFormat]
    lea rdx, [choice]
    call scanf

    mov eax, [choice]

    cmp eax, 1
    je pune

    cmp eax, 2
    je nashik

    cmp eax, 3
    je mumbai

    cmp eax, 4
    je exit_program

    lea rcx, [invalidMsg]
    call printf

    jmp menu_loop

pune:

    mov edx, 20
    jmp display_fare

nashik:

    mov edx, 50
    jmp display_fare

mumbai:

    mov edx, 80

display_fare:

    lea rcx, [fareFormat]
    call printf

    jmp menu_loop

exit_program:

    add rsp, 40
    xor eax, eax
    ret