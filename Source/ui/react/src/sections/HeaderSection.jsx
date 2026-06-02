import { Box, IconButton } from "@radix-ui/themes";
import { SettingsDialog } from "../components";
import { GearIcon } from "@radix-ui/react-icons";
import { ElementID } from "../utilities/juceBridge";
import PresetPanel from "./PresetPanel";

export default function HeaderSection({
  currentState,
  presets,
  selectedPresetIndex,
  setSelectedPresetIndex,
}) {
  return (
    <div
      style={{
        display: "flex",
        position: "relative",
        backgroundColor: "var(--bg-dark)",
        width: "100%",
        height: "65px",
        color: "white",
        fontFamily: "var(--font-sans)",
        fontWeight: "bold",
        alignItems: "center",
        justifyContent: "center",
      }}
    >
      {/* Settings dialog */}
      <SettingsDialog
        currentState={currentState}
        header="Settings"
        description=""
        buttonText="Accept"
        submitButtonText="Exit"
        triggerTooltip=""
        buttonStyle={{
          width: "25px",
          height: "25px",
          marginRight: "auto",
          marginLeft: "15px",
          color: "white",
          alignItems: "center",
          padding: "6px 6px",
        }}
        buttonVariant={"ghost"}
        icon={
          <GearIcon
            style={{
              width: "25px",
              height: "25px",
              transform: "translateY(2px)",
            }}
          />
        }
      />

      {}
      <div
        style={{
          position: "absolute",
          inset: 0,
          display: "flex",
          alignItems: "center",
          justifyContent: "end",
          pointerEvents: "none",
        }}
      >
        <div style={{ paddingRight: "5px", transform: "translateX(155px)" }}>
          Tremokitty!
        </div>
        <div
          style={{
            marginLeft: "auto",
            marginRight: "105px",
            pointerEvents: "auto",
          }}
        >
          <PresetPanel
            currentState={currentState}
            providedPresets={presets}
            selectedPresetIndex={selectedPresetIndex}
            setSelectedPresetIndex={setSelectedPresetIndex}
          />
        </div>
      </div>
    </div>
  );
}
