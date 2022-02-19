
Mode = {
	texture = 0,
	_3d = 1
}

NoiseType = {
	value = 0,
	perlin = 1,
	simplex = 2,
	simplex2S = 3,
	cellular = 4,
}

Config = {
	mode = Mode.texture,
	size = 1024,
	--fractal
	octaves = 1,
	amplitude = 1,
	frequency = 1.0,
	lacunarity = 2.0,
	persistence = 0.5,
	--noise
	seed = 0x5EED,
	noiseType = NoiseType.perlin,
	----cellular
	--distanceFn =,
	--returnType =,
	--jitter = 1.0f
}
