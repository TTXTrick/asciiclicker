const WebSocket = require("ws");
const wss = new WebSocket.Server({ port: 8080 });

let players = {};

wss.on("connection", ws => {
    const id = Math.floor(Math.random() * 99999);
    players[id] = { x:0, y:0, z:0 };

    ws.on("message", msg => {
        const parts = msg.toString().split(" ");
        
        if (parts[0] === "POS") {
            players[id].x = parseFloat(parts[1]);
            players[id].y = parseFloat(parts[2]);
            players[id].z = parseFloat(parts[3]);

            const update = `PLAYER ${id} ${parts[1]} ${parts[2]} ${parts[3]}`;
            for (let client of wss.clients)
                if (client.readyState === WebSocket.OPEN)
                    client.send(update);
        }
    });

    ws.on("close", () => {
        delete players[id];
    });
});

console.log("ASCIICKER online server running on ws://localhost:8080");
