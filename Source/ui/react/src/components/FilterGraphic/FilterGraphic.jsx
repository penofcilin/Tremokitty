import { useState } from "react";

function FilterGraphic({
  cutoff = 1,
  resonance = 0,
  modDepth = 0,
  filterType = 0,
}) {
  const WIDTH = 100;
  const HEIGHT = 60;

  const type = Number(filterType);
  const safeCutoff = Math.min(1, Math.max(0, cutoff));

  const cutoffX = safeCutoff * WIDTH;

  const MID_Y = HEIGHT * 0.5;

  const sweepWidth = modDepth * 40;
  const sweepX = Math.max(
    0,
    Math.min(WIDTH - sweepWidth, cutoffX - sweepWidth / 2)
  );

  // Vertical positions - centered with more space
  const TOP = HEIGHT * 0.25;
  const BOTTOM = HEIGHT * 0.75;

  // Resonance affects transition steepness and peak
  const baseTransition = 18;
  const TRANSITION = baseTransition * (1 - resonance * 0.65);

  // Resonance boost at cutoff
  const resBoost = resonance * HEIGHT * 0.18;

  /* Low-pass: high → drop at cutoff → low */
  const LP_PEAK = Math.max(TOP - resBoost, HEIGHT * 0.1);
  const LP_PATH = `
    M 0,${TOP}
    L ${Math.max(0, cutoffX - TRANSITION / 2)},${TOP}
    Q ${cutoffX - TRANSITION / 4},${LP_PEAK} ${cutoffX},${LP_PEAK}
    Q ${cutoffX + TRANSITION / 4},${LP_PEAK} ${
    cutoffX + TRANSITION / 2
  },${MID_Y}
    Q ${cutoffX + TRANSITION},${BOTTOM} ${Math.min(
    WIDTH,
    cutoffX + TRANSITION * 1.5
  )},${BOTTOM}
    L ${WIDTH},${BOTTOM}
  `;

  /* High-pass: low → rise at cutoff → high */
  const HP_PEAK = Math.max(TOP - resBoost, HEIGHT * 0.1);
  const HP_PATH = `
    M 0,${BOTTOM}
    L ${Math.max(0, cutoffX - TRANSITION * 1.5)},${BOTTOM}
    Q ${cutoffX - TRANSITION},${BOTTOM} ${cutoffX - TRANSITION / 2},${MID_Y}
    Q ${cutoffX - TRANSITION / 4},${HP_PEAK} ${cutoffX},${HP_PEAK}
    Q ${cutoffX + TRANSITION / 4},${HP_PEAK} ${Math.min(
    WIDTH,
    cutoffX + TRANSITION / 2
  )},${TOP}
    L ${WIDTH},${TOP}
  `;

  /* Band-pass: low → peak at cutoff → low */
  const BP_WIDTH = baseTransition * (1 - resonance * 0.5);
  const BP_PEAK = Math.max(TOP - resBoost, HEIGHT * 0.1);
  const BP_PATH = `
    M 0,${BOTTOM}
    L ${Math.max(0, cutoffX - BP_WIDTH)},${BOTTOM}
    Q ${cutoffX - BP_WIDTH / 2},${BP_PEAK} ${cutoffX},${BP_PEAK}
    Q ${cutoffX + BP_WIDTH / 2},${BP_PEAK} ${Math.min(
    WIDTH,
    cutoffX + BP_WIDTH
  )},${BOTTOM}
    L ${WIDTH},${BOTTOM}
  `;

  let curvePath;

  switch (type) {
    case 0:
      curvePath = LP_PATH;
      break;
    case 1:
      curvePath = HP_PATH;
      break;
    case 2:
      curvePath = BP_PATH;
      break;
    default:
      curvePath = LP_PATH;
  }

  return (
    <svg
      className="filterGraphic"
      viewBox={`0 0 ${WIDTH} ${HEIGHT}`}
      preserveAspectRatio="none"
    >
      <rect
        x="0"
        y="0"
        width={WIDTH}
        height={HEIGHT}
        fill="transparent"
        stroke="rgba(255,255,255,0.15)"
        strokeWidth="1"
        vectorEffect="non-scaling-stroke"
      />

      {modDepth > 0 && (
        <rect
          x={sweepX}
          y="0"
          width={sweepWidth}
          height={HEIGHT}
          fill="rgba(255,255,255,0.12)"
        />
      )}

      <path
        d={curvePath}
        fill="none"
        stroke="white"
        strokeWidth="1.5"
        strokeLinecap="round"
        strokeLinejoin="round"
        vectorEffect="non-scaling-stroke"
      />

      <line
        x1={cutoffX}
        x2={cutoffX}
        y1="0"
        y2={HEIGHT}
        stroke="rgba(255,255,255,0.6)"
        strokeWidth="1"
        vectorEffect="non-scaling-stroke"
      />
    </svg>
  );
}

export default function App() {
  const [cutoff, setCutoff] = useState(0.5);
  const [resonance, setResonance] = useState(0.3);
  const [modDepth, setModDepth] = useState(0.2);
  const [filterType, setFilterType] = useState(0);

  return (
    <div
      style={{
        padding: "40px",
        background: "#1a1a1a",
        minHeight: "100vh",
        color: "white",
      }}
    >
      <h1 style={{ marginBottom: "30px" }}>Filter Graphic Demo</h1>

      <div
        style={{
          marginBottom: "40px",
          background: "#2a2a2a",
          padding: "20px",
          borderRadius: "8px",
        }}
      >
        <FilterGraphic
          cutoff={cutoff}
          resonance={resonance}
          modDepth={modDepth}
          filterType={filterType}
        />
      </div>

      <div style={{ maxWidth: "400px" }}>
        <div style={{ marginBottom: "20px" }}>
          <label style={{ display: "block", marginBottom: "8px" }}>
            Filter Type
          </label>
          <select
            value={filterType}
            onChange={(e) => setFilterType(Number(e.target.value))}
            style={{
              width: "100%",
              padding: "8px",
              background: "#333",
              color: "white",
              border: "1px solid #555",
              borderRadius: "4px",
            }}
          >
            <option value={0}>Low Pass</option>
            <option value={1}>High Pass</option>
            <option value={2}>Band Pass</option>
          </select>
        </div>

        <div style={{ marginBottom: "20px" }}>
          <label style={{ display: "block", marginBottom: "8px" }}>
            Cutoff: {(cutoff * 100).toFixed(0)}%
          </label>
          <input
            type="range"
            min="0"
            max="1"
            step="0.01"
            value={cutoff}
            onChange={(e) => setCutoff(Number(e.target.value))}
            style={{ width: "100%" }}
          />
        </div>

        <div style={{ marginBottom: "20px" }}>
          <label style={{ display: "block", marginBottom: "8px" }}>
            Resonance: {(resonance * 100).toFixed(0)}%
          </label>
          <input
            type="range"
            min="0"
            max="1"
            step="0.01"
            value={resonance}
            onChange={(e) => setResonance(Number(e.target.value))}
            style={{ width: "100%" }}
          />
        </div>

        <div style={{ marginBottom: "20px" }}>
          <label style={{ display: "block", marginBottom: "8px" }}>
            Mod Depth: {(modDepth * 100).toFixed(0)}%
          </label>
          <input
            type="range"
            min="0"
            max="1"
            step="0.01"
            value={modDepth}
            onChange={(e) => setModDepth(Number(e.target.value))}
            style={{ width: "100%" }}
          />
        </div>
      </div>
    </div>
  );
}
