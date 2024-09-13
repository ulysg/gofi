
![Screenshot of gofi](https://github.com/user-attachments/assets/ec4f7305-1882-404c-90b9-d44b5df7c827)

# gofi

A simple app launcher inspired by rofi, using Adwaita 

## Usage

To run gofi, you need to have libadwaita and gtk4-layer-shell installed, and also use a Wayland compositor that support layer shell, like Sway, hyprland or Niri.

## Compilation

To compile gofi, you can simply use meson:

```
meson setup builddir
cd builddir
meson compile
```
