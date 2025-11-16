camTransform = Transform:new(Float3:new(0, 0, -5), Float3:new(1, 1, 1));
speed = 2

function start()
	Game.AddComponent(0, camTransform)
end

function update()

	if Input.GetKey(Key.W) then
		camTransform:AddPosition(Float3:new(0, 0, speed) * Time.GetDeltaTime())
	end

	if Input.GetKey(Key.S) then
		camTransform:AddPosition(Float3:new(0, 0, -speed) * Time.GetDeltaTime())
	end

	if Input.GetKey(Key.A) then
		camTransform:AddPosition(Float3:new(-speed, 0, 0) * Time.GetDeltaTime())
	end

	if Input.GetKey(Key.D) then
		camTransform:AddPosition(Float3:new(speed, 0, 0) * Time.GetDeltaTime())
	end

	Camera.SetPosition(camTransform.position)

	-- OutputDebug("Working...")
	-- OutputDebug("Entity ID: "..a.."")
end