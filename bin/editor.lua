-- editor.lua
world = {
  entities = {
    FlightHelmet = {
      path = "../data/models/gltf/FlightHelmet/glTF/FlightHelmet.gltf",
      position = {px = 0, py = 0, pz = 0},
      rotation = {rx = 0, ry = 180, rz = 0},
      scale = {sx = 1, sy = 1, sz = 1}
    },
    FlightHelmet2 = {
      path = "../data/models/gltf/FlightHelmet/glTF/FlightHelmet.gltf",
      position = {px = 1, py = 0, pz = 1},
      rotation = {rx = 0, ry = 180, rz = 0},
      scale = {sx = 1, sy = 1, sz = 1}
    }
  }
}

return world