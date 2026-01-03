import { TToggleGroup } from "../../components";
import {
  SineWaveIcon,
  CosineWaveIcon,
  NegativeCosineWaveIcon,
  SawWaveIcon,
  SawDownWaveIcon,
  SquareWaveIcon,
  InverseSquareWaveIcon,
  RandomWaveIcon,
} from "../../assets/icons";

export default function WaveSelector({
  id,
  value,
  onChange,
  vertical,
  itemSize = 18,
  style,
}) {
  return (
    <TToggleGroup
      id={id}
      value={value}
      onChange={onChange}
      style={style}
      vertical={vertical}
      options={[
        {
          value: "0",
          label: "Sine",
          icon: <SineWaveIcon width={itemSize} height={itemSize} />,
        },
        {
          value: "1",
          label: "Cosine",
          icon: <CosineWaveIcon width={itemSize} height={itemSize} />,
        },
        {
          value: "2",
          label: "Negative Cosine",
          icon: <NegativeCosineWaveIcon width={itemSize} height={itemSize} />,
        },
        {
          value: "3",
          label: "Saw",
          icon: <SawWaveIcon width={itemSize} height={itemSize} />,
        },
        {
          value: "4",
          label: "Saw Down",
          icon: <SawDownWaveIcon width={itemSize} height={itemSize} />,
        },
        {
          value: "5",
          label: "Square",
          icon: <SquareWaveIcon width={itemSize} height={itemSize} />,
        },
        {
          value: "6",
          label: "Inverse Square",
          icon: <InverseSquareWaveIcon width={itemSize} height={itemSize} />,
        },
        {
          value: "7",
          label: "Random",
          icon: <RandomWaveIcon width={itemSize} height={itemSize} />,
        },
      ]}
    />
  );
}
