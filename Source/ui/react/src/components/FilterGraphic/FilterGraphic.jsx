export default function FilterGraphic({
  cutoff = 1,
  resonance = 0,
  modDepth = 0,
  modRate = 1,
  filterType = 0,
  lfoPosition = 0, // -1 to 1, represents current LFO value
}) {
  const WIDTH = 100;
  const HEIGHT = 60;

  const type = Number(filterType);
  const safeCutoff = Math.min(1, Math.max(0, cutoff));

  // Normalize resonance from 0-10 range to 0-1 range
  const normalizedResonance = Math.min(1, Math.max(0, resonance / 10));

  const cutoffX = safeCutoff * WIDTH;

  const MID_Y = HEIGHT * 0.5;

  const sweepWidth = modDepth * WIDTH;
  const sweepX = Math.max(
    0,
    Math.min(WIDTH - sweepWidth, cutoffX - sweepWidth / 2),
  );

  // Vertical positions - high at middle, low extends off screen
  const TOP = HEIGHT * 0.45;
  const BOTTOM = HEIGHT * 1.3; // Extends below viewport

  // Resonance affects transition steepness and peak
  const baseTransition = 18;
  const TRANSITION = baseTransition * (1 - normalizedResonance * 0.65);

  // Resonance boost at cutoff
  const resBoost = normalizedResonance * HEIGHT * 0.18;

  /* Low-pass: high → drop at cutoff → low */
  const LP_PEAK = Math.max(TOP - resBoost, HEIGHT * 0.1);
  const LP_PATH = `
    M 0,${TOP}
    L ${Math.max(0, cutoffX - TRANSITION / 2)},${TOP}
    Q ${cutoffX - TRANSITION / 4},${LP_PEAK} ${cutoffX},${LP_PEAK}
    Q ${cutoffX + TRANSITION / 4},${LP_PEAK} ${
      cutoffX + TRANSITION / 2
    },${MID_Y}
    Q ${cutoffX + TRANSITION * 2},${BOTTOM * 0.9} ${Math.min(
      WIDTH,
      cutoffX + TRANSITION * 2.5,
    )},${BOTTOM}
    L ${WIDTH},${BOTTOM}
  `;

  /* High-pass: low → rise at cutoff → high */
  const HP_PEAK = Math.max(TOP - resBoost, HEIGHT * 0.1);
  const HP_PATH = `
    M 0,${BOTTOM}
    L ${Math.max(0, cutoffX - TRANSITION * 2.5)},${BOTTOM}
    Q ${cutoffX - TRANSITION * 2},${BOTTOM * 0.9} ${
      cutoffX - TRANSITION / 2
    },${MID_Y}
    Q ${cutoffX - TRANSITION / 4},${HP_PEAK} ${cutoffX},${HP_PEAK}
    Q ${cutoffX + TRANSITION / 4},${HP_PEAK} ${Math.min(
      WIDTH,
      cutoffX + TRANSITION / 2,
    )},${TOP}
    L ${WIDTH},${TOP}
  `;

  /* Band-pass: low → peak at cutoff → low */
  const BP_WIDTH = baseTransition * 2.4 * (1 - normalizedResonance * 0.7);
  const BP_PEAK = Math.max(TOP - resBoost * 1.5, HEIGHT * 0.05);
  const BP_PATH = `
    M 0,${BOTTOM}
    L ${Math.max(0, cutoffX - BP_WIDTH)},${BOTTOM}
    Q ${cutoffX - BP_WIDTH / 2},${BP_PEAK} ${cutoffX},${BP_PEAK}
    Q ${cutoffX + BP_WIDTH / 2},${BP_PEAK} ${Math.min(
      WIDTH,
      cutoffX + BP_WIDTH,
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

  // Calculate LFO indicator position and dimensions
  // lfoPosition ranges from -1 to 1
  // Map it to the sweep zone: -1 = left edge, 0 = center (cutoff), 1 = right edge
  const lfoOffset =
    lfoPosition !== undefined ? (lfoPosition * sweepWidth) / 2 : 0;
  const lfoX = cutoffX + lfoOffset;

  // Rectangle extends from cutoff to lfoX
  const rectX = Math.min(cutoffX, lfoX);
  const rectWidth = modRate > 0 ? Math.abs(lfoX - cutoffX) : 0;
  const rectY = TOP - resBoost - 3; // Just above the curve
  const rectHeight = 2;

  return (
    <svg
      className="filterGraphic"
      viewBox={`0 0 ${WIDTH} ${HEIGHT}`}
      preserveAspectRatio="none"
      width="300"
      height="90"
    >
      <rect
        x="0"
        y="0"
        width={WIDTH}
        height={HEIGHT}
        fill="transparent"
        stroke="rgba(255,255,255,0.15)"
        strokeWidth="0.5"
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
        strokeWidth="2"
        strokeLinecap="round"
        strokeLinejoin="round"
      />

      <line
        x1={cutoffX}
        x2={cutoffX}
        y1="0"
        y2={HEIGHT}
        stroke="rgba(255,255,255,0.6)"
        strokeWidth="0.5"
      />

      {/* LFO modulation indicator */}
      {modDepth > 0 && lfoPosition !== undefined && (
        <rect
          x={rectX}
          y={rectY}
          width={rectWidth}
          height={rectHeight}
          fill="rgba(255,220,0,0.9)"
          style={{ marginBottom: "10px" }}
        />
      )}

      <text
        x="2"
        y={HEIGHT - 2}
        fontSize="4"
        fill="rgba(255,255,255,0.6)"
        textAnchor="start"
        dominantBaseline="ideographic"
      >
        20
      </text>

      <text
        x={WIDTH - 2}
        y={HEIGHT - 2}
        fontSize="4"
        fill="rgba(255,255,255,0.6)"
        textAnchor="end"
        dominantBaseline="ideographic"
      >
        20 kHz
      </text>
    </svg>
  );
}
