import * as Dialog from "@radix-ui/react-dialog";
import { Button } from "@radix-ui/themes";
import { Cross2Icon } from "@radix-ui/react-icons";
import "./tDialog.css";

export default function TDialog({
  header,
  description,
  buttonText,
  fields = [],
  onSubmit,
}) {
  const handleSubmit = (e) => {
    e.preventDefault();
    if (!onSubmit) return;

    const data = new FormData(e.currentTarget);
    onSubmit(Object.fromEntries(data.entries()));
  };

  return (
    <Dialog.Root>
      <Dialog.Trigger asChild>
        <Button className="Button">{buttonText}</Button>
      </Dialog.Trigger>

      <Dialog.Portal>
        <Dialog.Overlay className="DialogOverlay" />

        <Dialog.Content className="DialogContent">
          <Dialog.Title className="DialogTitle">{header}</Dialog.Title>

          <Dialog.Description className="DialogDescription">
            {description}
          </Dialog.Description>

          <form onSubmit={handleSubmit}>
            {fields.map((field) => (
              <fieldset className="Fieldset" key={field.name}>
                <label className="Label" htmlFor={field.name}>
                  {field.label}
                </label>

                <input
                  className="Input"
                  id={field.name}
                  name={field.name}
                  type={field.type ?? "text"}
                  defaultValue={field.defaultValue}
                />
              </fieldset>
            ))}

            <div
              style={{
                display: "flex",
                marginTop: 25,
                justifyContent: "flex-end",
              }}
            >
              <Dialog.Close asChild>
                <button className="Button save" type="submit">
                  Save
                </button>
              </Dialog.Close>
            </div>
          </form>

          <Dialog.Close asChild>
            <button className="IconButton" aria-label="Close">
              <Cross2Icon />
            </button>
          </Dialog.Close>
        </Dialog.Content>
      </Dialog.Portal>
    </Dialog.Root>
  );
}
