local camSpeed = 5
local camRotationSpeed = 12
local deg2rad = math.pi / 180

local HealthComponent = {
	health = 100,
	maxHealth = 1000
}

function awake()
	Game.RegisterComponent("Health",  HealthComponent)
	Game.AddComponent(0, "Health", { health = 1 })
end

function update()

	-- myTransform = Game.GetComponent(0, "Transform")
	health = Game.GetComponent(0, "Health")

	move_cam()

end

pitch = 0
yaw = 0

function clamp(value, min, max)
    return math.min(math.max(value, min), max)
end

function move_cam()
	
	pos = Camera.GetPosition()
	rot = Camera.GetRotation()

	mousePos = Input.GetMouseDelta()
	
	pitch = pitch + mousePos.y * camRotationSpeed * Time.GetDeltaTime()
	yaw = yaw + mousePos.x * camRotationSpeed * Time.GetDeltaTime()
	
	pitch = clamp(pitch, -75, 75)
	
	if Input.GetKey(Key.W) then
		pos = pos + Camera.GetRotation():Forward() * camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.S) then
		pos = pos + Camera.GetRotation():Forward() * -camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.A) then
		pos = pos + Camera.GetRotation():Right() * -camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.D) then
		pos = pos + Camera.GetRotation():Right() * camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.SPACE) then
		pos = pos + Float3:Up() * camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.C) then
		pos = pos + Float3:Up() * -camSpeed * Time.GetDeltaTime()
	end
	
	local deg2rad = math.pi / 180
	local yawRad = yaw * deg2rad
	local pitchRad = pitch * deg2rad
	
	local rotationY = Quaternion.FromAxisAngle(Float3:new(0, 1, 0), yawRad)
	local rotationX = Quaternion.FromAxisAngle(Float3:Right(), pitchRad)
	
	local finalQuat = rotationY * rotationX
	
	finalQuat:Normalize()

	Camera.SetPosition(pos)
	Camera.SetRotation(finalQuat)

end