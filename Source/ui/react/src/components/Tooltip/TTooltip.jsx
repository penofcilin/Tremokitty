import * as Tooltip from "@radix-ui/react-tooltip";
import "./TTooltip.css";

export default function TTooltip({
  content,
  children,
  side = "top",
  delay = 600,
}) {
  if (!content) return children;

  return (
    <Tooltip.Provider delayDuration={delay}>
      <Tooltip.Root>
        <Tooltip.Trigger asChild>{children}</Tooltip.Trigger>
        <Tooltip.Portal>
          <Tooltip.Content
            className="TooltipContent"
            side={side}
            sideOffset={6}
          >
            {content}
            <Tooltip.Arrow className="TooltipArrow" />
          </Tooltip.Content>
        </Tooltip.Portal>
      </Tooltip.Root>
    </Tooltip.Provider>
  );
}
