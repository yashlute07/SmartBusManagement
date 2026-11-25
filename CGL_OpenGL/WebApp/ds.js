// SMART BUS - DATA STRUCTURES
// Browser-based implementation of DS practical algorithms

let stack = [];
let queue = [];
let hashTable = new Array(10).fill(null);

// STACK - LIFO
function stackOperation(action, value) {
    switch (action) {
        case "push":
            stack.push(value);
            return "Pushed: " + value;
        case "pop":
            return stack.length ? "Popped: " + stack.pop() : "Stack Underflow";
        case "peek":
            return stack.length ? "Top: " + stack[stack.length - 1] : "Stack is Empty";
        case "display":
            return stack.length ? "Stack: " + stack.slice().reverse().join(" | ") : "Stack is Empty";
        case "reset":
            stack = [];
            return "Stack Reset Successful";
    }
}

// QUEUE - FIFO
function queueOperation(action, value) {
    switch (action) {
        case "enqueue":
            queue.push(value);
            return "Enqueued: " + value;
        case "dequeue":
            return queue.length ? "Dequeued: " + queue.shift() : "Queue Underflow";
        case "peek":
            return queue.length ? "Front: " + queue[0] : "Queue is Empty";
        case "display":
            return queue.length ? "Queue: " + queue.join(" -> ") : "Queue is Empty";
        case "reset":
            queue = [];
            return "Queue Reset Successful";
    }
}

// HASHING - LINEAR PROBING
function hashOperation(action, value) {
    if (action === "display") {
        return hashTable.map((v, i) => i + ": " + (v === null ? "Empty" : v)).join("\n");
    }

    if (action === "reset") {
        hashTable.fill(null);
        return "Hash Table Reset Successful";
    }

    const hash = value.split("").reduce(
        (sum, ch) => sum + ch.charCodeAt(0), 0
    ) % 10;

    if (action === "insert") {
        for (let i = 0; i < 10; i++) {
            const index = (hash + i) % 10;

            if (hashTable[index] === value)
                return "Value already exists at index " + index;

            if (hashTable[index] === null) {
                hashTable[index] = value;
                return "Inserted " + value + " at index " + index;
            }
        }
        return "Hash Table is Full";
    }

    if (action === "search") {
        for (let i = 0; i < 10; i++) {
            const index = (hash + i) % 10;

            if (hashTable[index] === value)
                return "Found " + value + " at index " + index;

            if (hashTable[index] === null) break;
        }
        return "Value Not Found";
    }
}

// SORTING
function sortingOperation(value, algorithm) {
    if (!value.trim()) return "Please enter numbers.";

    let arr = value.trim().split(/[\s,]+/).map(Number);

    if (arr.some(n => !Number.isFinite(n)))
        return "Enter valid numbers separated by commas or spaces.";

    switch (algorithm) {
        case "bubble":
            for (let i = 0; i < arr.length - 1; i++)
                for (let j = 0; j < arr.length - i - 1; j++)
                    if (arr[j] > arr[j + 1])
                        [arr[j], arr[j + 1]] = [arr[j + 1], arr[j]];
            break;

        case "selection":
            for (let i = 0; i < arr.length - 1; i++) {
                let min = i;
                for (let j = i + 1; j < arr.length; j++)
                    if (arr[j] < arr[min]) min = j;
                [arr[i], arr[min]] = [arr[min], arr[i]];
            }
            break;

        case "insertion":
            for (let i = 1; i < arr.length; i++) {
                let key = arr[i];
                let j = i - 1;

                while (j >= 0 && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    j--;
                }
                arr[j + 1] = key;
            }
            break;
    }

    return "Sorted Output: " + arr.join("  ");
}

// WEBSITE BUTTON HANDLER
function runDS(type, action) {
    let value = "";
    let output = "";

    if (type === "stack") {
        value = document.getElementById("stackValue").value.trim();
        if (action === "push" && !value) {
            output = "Please enter a value.";
        } else {
            output = stackOperation(action, value);
        }
        document.getElementById("stackOutput").textContent = output;
    }

    if (type === "queue") {
        value = document.getElementById("queueValue").value.trim();
        if (action === "enqueue" && !value) {
            output = "Please enter a value.";
        } else {
            output = queueOperation(action, value);
        }
        document.getElementById("queueOutput").textContent = output;
    }

    if (type === "hash") {
        value = document.getElementById("hashValue").value.trim();
        if (["insert", "search"].includes(action) && !value) {
            output = "Please enter a value.";
        } else {
            output = hashOperation(action, value);
        }
        document.getElementById("hashOutput").textContent = output;
    }

    if (type === "sort") {
        value = document.getElementById("sortValues").value;
        const algorithm = document.getElementById("sortAlgorithm").value;
        output = sortingOperation(value, algorithm);
        document.getElementById("sortOutput").textContent = output;
    }
}
