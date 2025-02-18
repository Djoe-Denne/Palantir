import os
import shutil

# Define the project root
project_root = "."

# Define the new structure mapping
structure_mapping = {
    "src/ICommand.cpp": "include/command/icommand.hpp",
    "src/ShowCommand.cpp": "include/command/show_command.hpp",
    "src/StopCommand.cpp": "include/command/stop_command.hpp",
    "src/IInput.cpp": "include/input/iinput.hpp",
    "src/ISignal.cpp": "include/signal/isignal.hpp",
    "src/CtrlF1Signal.cpp": "include/signal/ctrl_f1_signal.hpp",
    "src/IWindow.cpp": "include/window/iwindow.hpp",
    "src/WindowManager.cpp": "include/window/window_manager.hpp",
    "src/SignalManager.cpp": "include/window/signal_manager.hpp",
    "src/Input.cpp": "include/windows/input/input.hpp",
    "src/OverlayWindow.cpp": "include/windows/window/overlay_window.hpp",
}

# Duplicate for src directory
src_mapping = {f"src/{key.split('/')[-1]}": f"src/{value[8:]}" for key, value in structure_mapping.items()}

# Merge both mappings
structure_mapping.update(src_mapping)

# Function to move files
def move_files():
    for old_path, new_path in structure_mapping.items():
        old_full_path = os.path.join(project_root, old_path)
        new_full_path = os.path.join(project_root, new_path)

        if os.path.exists(old_full_path):
            os.makedirs(os.path.dirname(new_full_path), exist_ok=True)
            shutil.move(old_full_path, new_full_path)
            print(f"Moved: {old_path} -> {new_path}")
        else:
            print(f"Skipped (not found): {old_path}")

if __name__ == "__main__":
    move_files()
