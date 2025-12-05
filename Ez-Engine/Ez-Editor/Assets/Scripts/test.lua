local camSpeed = 5
local camRotationSpeed = 12
local deg2rad = math.pi / 180

local MoveComponent = {
	moveSpeed = 4
}

function awake()
	Game.RegisterComponent("Move",  MoveComponent)
	Game.AddComponent(1, "Move", {})
end

function update()

	myTransform = Game.GetComponent(1, "Transform")

	move_cam()

end

local pitch = 0
local yaw = 0

local function clamp(value, min, max)
    return math.min(math.max(value, min), max)
end

function move_cam()
	
	camTransform = Game.GetComponent(Camera.GetEntity(), "Transform")

	mousePos = Input.GetMouseDelta()
	
	pitch = pitch + mousePos.y * camRotationSpeed * Time.GetDeltaTime()
	yaw = yaw + mousePos.x * camRotationSpeed * Time.GetDeltaTime()
	
	pitch = clamp(pitch, -75, 75)
	
	if Input.GetKey(Key.W) then
		camTransform.position = camTransform.position + camTransform.rotation:Forward() * camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.S) then
		camTransform.position = camTransform.position + camTransform.rotation:Forward() * -camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.A) then
		camTransform.position = camTransform.position + camTransform.rotation:Right() * -camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.D) then
		camTransform.position = camTransform.position + camTransform.rotation:Right() * camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.SPACE) then
		camTransform.position = camTransform.position + Float3:Up() * camSpeed * Time.GetDeltaTime()
	end
	
	if Input.GetKey(Key.C) then
		camTransform.position = camTransform.position + Float3:Up() * -camSpeed * Time.GetDeltaTime()
	end
	
	local deg2rad = math.pi / 180
	local yawRad = yaw * deg2rad
	local pitchRad = pitch * deg2rad
	
	local rotationY = Quaternion.FromAxisAngle(Float3:new(0, 1, 0), yawRad)
	local rotationX = Quaternion.FromAxisAngle(Float3:Right(), pitchRad)
	
	local finalQuat = rotationY * rotationX
	
	finalQuat:Normalize()

	camTransform.rotation = finalQuat

end