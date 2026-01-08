import { PARAM_DEFAULTS } from "./juceBridge";

export function toPercentage(n) {
  return `${Math.round(n * 100)}%`;
}

export function normToSkewed(n, min, max, skew) {
  const clamped = Math.min(1, Math.max(0, n));
  const exponent = skew === 1 ? 1 : 1 / skew;
  return min + (max - min) * Math.pow(clamped, exponent);
}

export function parseParams(initialData) {
  // initialData is an array with 1 XML string
  const xmlString = initialData[0];

  const parser = new DOMParser();
  const xml = parser.parseFromString(xmlString, "text/xml");

  const params = xml.querySelectorAll("PARAM");

  return [...params].map((param) => ({
    id: param.getAttribute("id"),
    value: param.hasAttribute("value")
      ? Number(param.getAttribute("value"))
      : undefined,
  }));
}

export function resolveDefaults(parsedParams) {
  const resolved = { ...PARAM_DEFAULTS };

  for (const { id, value } of parsedParams) {
    if (value !== undefined) {
      resolved[id] = value;
    } else if (!(id in resolved)) {
      // safety for new params added later
      resolved[id] = undefined;
    }
  }

  return resolved;
}
