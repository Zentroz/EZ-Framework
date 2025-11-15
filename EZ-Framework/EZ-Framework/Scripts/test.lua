position = Float3:new()

-- a = Game.CreateEntity();

function update(dt)
	if Input.GetKey(Key.SPACE) then
	   OutputDebug("Entity ID: "..Game.CreateEntity().."")	
	end

	if Input.GetKey(Key.R) then
	   Game.DestroyEntity(0)
	end

	if Input.GetKey(Key.G) then
		local myArray = { 0, 1, 2, 4, 7, 65, 1002 }
	   Game.GetEntitiesWithComponents(myArray);
	end

	-- OutputDebug("Working...")
	-- OutputDebug("Entity ID: "..a.."")
end