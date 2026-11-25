const pipelineInstructions = [
    "LOAD R1, 100",
    "LOAD R2, 3",
    "MUL R3, R1, R2",
    "STORE R3, FARE",
    "HALT"
];

let pipelineTimer = null;
let pipelineCycle = 0;

function runCOA4Control() {
    const output = document.getElementById("coa4ControlOutput");
    const instruction = document.getElementById("coa4Instruction").value;
    const stages = ["Fetch", "Decode", "Execute", "Memory", "Write Back"];

    output.innerHTML =
        "<strong>Instruction:</strong> " + instruction +
        "<br><br>" + stages.map((stage, i) =>
            (i + 1) + ". " + stage + " - Completed"
        ).join("<br>");
}

function startCOA4Pipeline() {
    clearInterval(pipelineTimer);
    pipelineCycle = 0;

    const output = document.getElementById("coa4PipelineOutput");
    const cycle = document.getElementById("coa4Cycle");

    output.innerHTML = "Pipeline started...";

    pipelineTimer = setInterval(() => {
        pipelineCycle++;
        cycle.textContent = pipelineCycle;

        const completed = Math.min(pipelineCycle, pipelineInstructions.length);
        let html = "";

        pipelineInstructions.forEach((instruction, i) => {
            const stage = pipelineCycle - i;
            const names = ["IF", "ID", "EX", "MEM", "WB"];

            html += "<p><strong>" + instruction + "</strong>: ";

            if (stage >= 1 && stage <= 5) {
                html += names[stage - 1];
            } else if (stage > 5) {
                html += "Completed";
            } else {
                html += "Waiting";
            }

            html += "</p>";
        });

        output.innerHTML = html;

        if (pipelineCycle >= pipelineInstructions.length + 5) {
            clearInterval(pipelineTimer);
            output.innerHTML += "<strong>Pipeline execution completed!</strong>";
        }
    }, 900);
}

function resetCOA4Pipeline() {
    clearInterval(pipelineTimer);
    pipelineCycle = 0;
    document.getElementById("coa4Cycle").textContent = "0";
    document.getElementById("coa4PipelineOutput").textContent =
        "Pipeline reset. Ready to start.";
}

function showCOA4Hazard() {
    document.getElementById("coa4HazardOutput").textContent =
        "Data Hazard (RAW): An instruction may need a result that a previous instruction has not written yet. Forwarding or stalling can resolve this.";
}
