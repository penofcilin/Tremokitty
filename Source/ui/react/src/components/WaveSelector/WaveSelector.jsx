import { TToggleGroup } from "../../components";
import {
  SineWaveIcon,
  CosineWaveIcon,
  NegativeCosineWaveIcon,
  SawWaveIcon,
  SawDownWaveIcon,
  SquareWaveIcon,
  InverseSquareWaveIcon,
} from "../../assets/icons";

export default function WaveSelector({ id, value, onChange, vertical, style }) {
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
          icon: <SineWaveIcon width={18} height={18} />,
        },
        {
          value: "1",
          label: "Cosine",
          icon: <CosineWaveIcon width={18} height={18} />,
        },
        {
          value: "2",
          label: "Negative Cosine",
          icon: <NegativeCosineWaveIcon width={18} height={18} />,
        },
        {
          value: "3",
          label: "Saw",
          icon: <SawWaveIcon width={20} height={20} />,
        },
        {
          value: "4",
          label: "Saw Down",
          icon: <SawDownWaveIcon width={20} height={20} />,
        },
        {
          value: "5",
          label: "Square",
          icon: <SquareWaveIcon width={18} height={18} />,
        },
        {
          value: "6",
          label: "Inverse Square",
          icon: <InverseSquareWaveIcon width={18} height={18} />,
        },
      ]}
    />
  );
}
