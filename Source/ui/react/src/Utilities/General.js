export function toPercentage(n) {
  return `${Math.round(n * 100)}%`;
}

export function normToSkewed(n, min, max, skew) {
  const clamped = Math.min(1, Math.max(0, n));
  const exponent = skew === 1 ? 1 : 1 / skew;
  return min + (max - min) * Math.pow(clamped, exponent);
}
