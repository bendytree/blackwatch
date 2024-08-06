

export const roundTo = (val: number, digits: number): number => {
  const factor = Math.pow(10, digits);
  return Math.round(val * factor) / factor;
};

export const clamp = (val:number, min:number, max:number) => {
  return Math.max(min, Math.min(val, max));
};
