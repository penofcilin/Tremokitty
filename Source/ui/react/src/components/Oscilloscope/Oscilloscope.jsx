import { useEffect, useRef } from "react";

export default function Oscilloscope({
  lfoValue = 0,
  depth = 1,
  rate = 1,
  style,
  width = 100,
  height = 100,
  bypassed = false,
}) {
  const canvasRef = useRef(null);

  const WIDTH = width;
  const HEIGHT = height;
  const SNAPSHOT_SIZE = 24;

  const bufferRef = useRef(new Array(SNAPSHOT_SIZE).fill(0));
  const writeIndex = useRef(0);
  const currentLfoRef = useRef(0);
  const currentDepthRef = useRef(1);
  const currentRateRef = useRef(1);

  if (bypassed) {
    lfoValue = 0;
  }

  // update current LFO value, depth, and rate when they change
  useEffect(() => {
    currentLfoRef.current = lfoValue;
  }, [lfoValue]);

  useEffect(() => {
    currentDepthRef.current = depth;
  }, [depth]);

  useEffect(() => {
    currentRateRef.current = rate;
  }, [rate]);

  // continuous rendering loop
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext("2d");

    let animationId;
    let lastTime = performance.now();
    const SCROLL_SPEED = 60; // pixels per second

    const render = (currentTime) => {
      const deltaTime = (currentTime - lastTime) / 1000;
      lastTime = currentTime;

      // advance write position based on scroll speed (always scroll, even when rate is 0)
      const pixelsToAdvance = SCROLL_SPEED * deltaTime;
      const samplesPerPixel = SNAPSHOT_SIZE / WIDTH;
      const samplesToAdvance = pixelsToAdvance * samplesPerPixel;

      // write current LFO value multiple times if needed
      for (let i = 0; i < Math.ceil(samplesToAdvance); i++) {
        // when rate is 0, write 0 to flatten the wave
        const sample = currentRateRef.current === 0 ? 0 : currentLfoRef.current;

        bufferRef.current[writeIndex.current] = sample;
        writeIndex.current = (writeIndex.current + 1) % SNAPSHOT_SIZE;
      }

      const styles = getComputedStyle(document.documentElement);
      const bg = styles.getPropertyValue("--bg-darkish") || "#111";
      const stroke = styles.getPropertyValue("--stroke") || "#fff";

      ctx.clearRect(0, 0, WIDTH, HEIGHT);
      ctx.fillStyle = bg;
      ctx.fillRect(0, 0, WIDTH, HEIGHT);

      // draw midpoint line
      ctx.strokeStyle = stroke;
      ctx.globalAlpha = 0.3;
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.moveTo(0, HEIGHT / 2);
      ctx.lineTo(WIDTH, HEIGHT / 2);
      ctx.stroke();
      ctx.globalAlpha = 1;

      // draw smooth wave using quadratic curves
      ctx.strokeStyle = stroke;
      ctx.lineWidth = 2;
      ctx.beginPath();

      const points = [];
      for (let i = 0; i < SNAPSHOT_SIZE; i++) {
        const index = (writeIndex.current + i) % SNAPSHOT_SIZE;
        const x = (i / (SNAPSHOT_SIZE - 1)) * WIDTH;
        const y =
          HEIGHT / 2 -
          bufferRef.current[index] * currentDepthRef.current * (HEIGHT * 0.45);
        points.push({ x, y });
      }

      ctx.moveTo(points[0].x, points[0].y);

      // use quadratic curves for smoothness
      for (let i = 0; i < points.length - 1; i++) {
        const xc = (points[i].x + points[i + 1].x) / 2;
        const yc = (points[i].y + points[i + 1].y) / 2;
        ctx.quadraticCurveTo(points[i].x, points[i].y, xc, yc);
      }

      // connect to last point
      const last = points[points.length - 1];
      ctx.lineTo(last.x, last.y);
      ctx.stroke();

      // draw modulation circle at the end
      ctx.fillStyle = stroke;
      ctx.beginPath();
      ctx.arc(last.x, last.y, 4, 0, Math.PI * 2);
      ctx.fill();

      animationId = requestAnimationFrame(render);
    };

    if (!bypassed) render(performance.now());

    return () => {
      if (animationId) cancelAnimationFrame(animationId);
    };
  }, []);

  return (
    <div className="oscilloscopeView" style={style}>
      <canvas ref={canvasRef} width={WIDTH} height={HEIGHT} />
    </div>
  );
}
