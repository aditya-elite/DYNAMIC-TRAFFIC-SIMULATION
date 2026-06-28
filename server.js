const express = require('express');
const { exec, execSync } = require('child_process');
const path = require('path');

/* ── Compile C engine at startup ── */
try {
    console.log('Compiling C simulation engine...');
    execSync('gcc sim.c -o sim.exe');
    console.log('Compilation successful!');
} catch (e) {
    console.error('Failed to compile sim.c — make sure gcc is installed.\n', e.message);
}

const app = express();
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (_req, res) => {
    res.set('Cache-Control', 'no-store, no-cache, must-revalidate, private');
    res.sendFile(path.join(__dirname, 'index.html'));
});

/*
 * POST /get-traffic
 *
 * Expected body (all fields optional, default 0):
 *   h_active, v_active   — vehicles currently in/near intersection
 *   emer                 — 0 | 1 (H emergency) | 2 (V emergency)
 *   current_phase        — 1 | 2
 *   time_in_phase        — seconds elapsed in current phase
 *   h_tot, v_tot         — total vehicles on each road
 *   h_left, h_right      — turning vehicles on H road (optional, pass 0)
 *
 * Response JSON: { phase, duration_left, speed, congestion }
 */
app.post('/get-traffic', (req, res) => {
    const {
        l1_act, l2_act, l3_act, l4_act,
        l1_tot, l2_tot, l3_tot, l4_tot,
        emer, current_phase, time_in_phase
    } = req.body;

    const args = [
        l1_act ?? 0, l2_act ?? 0, l3_act ?? 0, l4_act ?? 0,
        l1_tot ?? 0, l2_tot ?? 0, l3_tot ?? 0, l4_tot ?? 0,
        emer ?? 0,
        current_phase ?? 1,
        time_in_phase ?? 0
    ].join(' ');

    const simExe = path.join(__dirname, 'sim.exe');
    exec(`"${simExe}" ${args}`, (error, stdout, stderr) => {
        if (error) {
            console.error('C execution error:', error.message, stderr);
            return res.status(500).json({ error: 'Simulation failed' });
        }

        try {
            const parts = stdout.trim().split(',');
            res.json({
                phase: parseInt(parts[0]),
                duration_left: parseInt(parts[1]),
                speed: parseFloat(parts[2]),
                congestion: parseInt(parts[3]),
            });
        } catch (e) {
            console.error('Parse error:', e.message, '| raw:', stdout);
            res.status(500).json({ error: 'Invalid simulation output' });
        }
    });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () =>
    console.log(`🚦 Smart Traffic System live → http://localhost:${PORT}`)
);