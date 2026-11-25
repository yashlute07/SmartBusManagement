
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

main:
    sub rsp, 40

    lea rcx, [title]
    call printf

    ; Passenger name input
    lea rcx, [namePrompt]
    call printf

    lea rcx, [inputName]
    lea rdx, [passengerName]
    call scanf

    ; Count characters in passenger name
    lea rsi, [passengerName]
    xor ecx, ecx

count_loop:
    movzx eax, byte [rsi]
    test al, al
    jz count_done

    inc ecx
    inc rsi
    jmp count_loop

count_done:
    mov [nameLength], ecx

    ; Destination selection
    lea rcx, [destMenu]
    call printf

    lea rcx, [inputInt]
    lea rdx, [choice]
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
    call printf

    lea rcx, [inputInt]
    lea rdx, [distance]
    call scanf

    mov eax, [distance]

    cmp eax, 1
    jl bad_distance

    ; Dynamic fare calculation
    ; Fare = Distance * 3

    imul eax, 3
    mov [fare], eax

    ; Accept passenger count
    lea rcx, [countPrompt]
    call printf

    lea rcx, [inputInt]
    lea rdx, [passengerCount]
    call scanf

    mov eax, [passengerCount]

    cmp eax, 1
    jl bad_count

    ; Total = Fare * Passenger Count

    imul eax, dword [fare]
    mov [total], eax

    ; Display passenger name
    lea rcx, [nameOutput]
    lea rdx, [passengerName]
    call printf

    ; Display name character count
    lea rcx, [lengthOut]
    mov edx, [nameLength]
    call printf

    ; Display distance
    lea rcx, [distanceOut]
    mov edx, [distance]
    call printf

    ; Display rate
    lea rcx, [rateOut]
    call printf

    ; Display fare per passenger
    lea rcx, [fareOut]
    mov edx, [fare]
    call printf

    ; Display total fare
    lea rcx, [totalOut]
    mov edx, [total]
    call printf

    jmp finish

invalid:
    lea rcx, [invalidMsg]
    call printf
    jmp finish

bad_distance:
    lea rcx, [distanceError]
    call printf
    jmp finish

bad_count:
    lea rcx, [countError]
    call printf

finish:
    add rsp, 40
    xor eax, eax
    ret
