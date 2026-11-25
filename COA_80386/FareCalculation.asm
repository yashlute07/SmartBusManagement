
default rel

extern printf
extern scanf
global main

section .data
    title       db 10, "=== SMART BUS: COA CO3 ===", 10, 0

    namePrompt  db "Enter passenger name (no spaces): ", 0

    destMenu    db 10, "1. Pune", 10
                db "2. Nashik", 10
                db "3. Mumbai", 10
                db "Enter destination choice: ", 0

    distancePrompt db "Enter distance in KM: ", 0
    countPrompt db "Enter number of passengers: ", 0

    inputInt    db "%d", 0
    inputName   db "%31s", 0

    nameOutput  db 10, "Passenger name: %s", 10, 0
    lengthOut   db "Name character count: %d", 10, 0
    distanceOut db "Distance: %d KM", 10, 0
    rateOut     db "Fare per KM: Rs. 3", 10, 0
    fareOut     db "Fare per passenger: Rs. %d", 10, 0
    totalOut    db "Total fare: Rs. %d", 10, 0

    invalidMsg  db "Invalid destination choice.", 10, 0
    distanceError db "Distance must be positive.", 10, 0
    countError  db "Passenger count must be positive.", 10, 0

section .bss
    passengerName resb 32
    choice         resd 1
    distance       resd 1
    passengerCount resd 1
    fare           resd 1
    total          resd 1
    nameLength     resd 1

section .text

; ==========================================
; BOOTH'S MULTIPLICATION ALGORITHM
; Input:
; ECX = Multiplicand (M)
; EDX = Multiplier (Q)
; Output:
; EAX = Signed multiplication result
; ==========================================

booth_mul:

    xor r8d, r8d       ; A = 0
    mov r9d, edx       ; Q = Multiplier
    mov r10d, ecx      ; M = Multiplicand
    xor r11d, r11d     ; Q-1 = 0

    mov eax, 32        ; 32-bit iterations

booth_loop:

    ; Check Q0 and Q-1

    mov edx, r9d
    and edx, 1

    cmp edx, r11d
    je booth_shift

    cmp edx, 0
    jne booth_subtract

    ; Q0 Q-1 = 01
    ; A = A + M

    add r8d, r10d
    jmp booth_shift

booth_subtract:

    ; Q0 Q-1 = 10
    ; A = A - M

    sub r8d, r10d

booth_shift:

    ; Save previous Q0 into Q-1

    mov r11d, r9d
    and r11d, 1

    ; Arithmetic right shift of A:Q

    mov edx, r8d
    shl edx, 31

    shr r9d, 1
    or r9d, edx

    sar r8d, 1

    ; Repeat 32 times

    dec eax
    jnz booth_loop

    ; Final result is stored in Q

    mov eax, r9d
    ret


; ==========================================
; MAIN PROGRAM
; ==========================================

main:

    sub rsp, 40

    lea rcx, [title]
    xor eax, eax
    call printf

    ; Passenger name input

    lea rcx, [namePrompt]
    xor eax, eax
    call printf

    lea rcx, [inputName]
    lea rdx, [passengerName]
    xor eax, eax
    call scanf

    ; Count characters in passenger name

    lea r8, [passengerName]
    xor r9d, r9d

count_loop:

    movzx eax, byte [r8]
    test al, al
    jz count_done

    inc r9d
    inc r8
    jmp count_loop

count_done:

    mov [nameLength], r9d

    ; Destination selection

    lea rcx, [destMenu]
    xor eax, eax
    call printf

    lea rcx, [inputInt]
    lea rdx, [choice]
    xor eax, eax
    call scanf

    mov eax, [choice]

    cmp eax, 1
    je get_distance

    cmp eax, 2
    je get_distance

    cmp eax, 3
    je get_distance

    jmp invalid

get_distance:

    ; Accept distance

    lea rcx, [distancePrompt]
    xor eax, eax
    call printf

    lea rcx, [inputInt]
    lea rdx, [distance]
    xor eax, eax
    call scanf

    mov eax, [distance]

    cmp eax, 1
    jl bad_distance

    ; ======================================
    ; FARE = DISTANCE * 3
    ; Using Booth's Algorithm
    ; ======================================

    mov ecx, [distance]
    mov edx, 3

    call booth_mul

    mov [fare], eax

    ; Accept passenger count

    lea rcx, [countPrompt]
    xor eax, eax
    call printf

    lea rcx, [inputInt]
    lea rdx, [passengerCount]
    xor eax, eax
    call scanf

    mov eax, [passengerCount]

    cmp eax, 1
    jl bad_count

    ; ======================================
    ; TOTAL = FARE * PASSENGER COUNT
    ; Using Booth's Algorithm
    ; ======================================

    mov ecx, [fare]
    mov edx, [passengerCount]

    call booth_mul

    mov [total], eax

    ; Display passenger name

    lea rcx, [nameOutput]
    lea rdx, [passengerName]
    xor eax, eax
    call printf

    ; Display name character count

    lea rcx, [lengthOut]
    mov edx, [nameLength]
    xor eax, eax
    call printf

    ; Display distance

    lea rcx, [distanceOut]
    mov edx, [distance]
    xor eax, eax
    call printf

    ; Display rate

    lea rcx, [rateOut]
    xor eax, eax
    call printf

    ; Display fare per passenger

    lea rcx, [fareOut]
    mov edx, [fare]
    xor eax, eax
    call printf

    ; Display total fare

    lea rcx, [totalOut]
    mov edx, [total]
    xor eax, eax
    call printf

    jmp finish

invalid:

    lea rcx, [invalidMsg]
    xor eax, eax
    call printf
    jmp finish

bad_distance:

    lea rcx, [distanceError]
    xor eax, eax
    call printf
    jmp finish

bad_count:

    lea rcx, [countError]
    xor eax, eax
    call printf

finish:

    add rsp, 40
    xor eax, eax
    ret
