out_name = getSaveFileName caption:"Export to EO File" types:"EO Files(*.eo)|*.eo" initialDir:(getDir #export) historyCategory:"ExportTextFiles"
	
if out_name != undefined then 
(
	file = fopen out_name "wb"
	
	num_objects = 0
	
	for obj in objects do
	(
		num_objects = num_objects + 1
	)
	
	writeLong file num_objects
	
	for object in objects do
	(		
		pos = object.position
		rot = object.rotation
			
		mesh = object.mesh
	
		local zRot = rotateXMatrix -90
		
		pos = pos * zRot
		rot = rot * zRot
		
		local WorldMatrix = copy object.transform
			
		local CM = ( object.transform * ( inverse WorldMatrix ) ) * zRot

		writeLong file mesh.numverts 		
		writeLong file mesh.numtverts
		writeLong file mesh.numfaces
		
		-- write position
		writeFloat file pos.x
		writeFloat file pos.y
		writeFloat file pos.z
		
		-- write euler angles
		euler = quattoeuler( rot ) --order:6
				
		writeFloat file euler.x  
		writeFloat file euler.y
		writeFloat file euler.z	
		
		-- write name
		writeString file object.name
		
		material = object.material
		
		n_submtl    = getNumSubmtls material 

		writeLong file n_submtl
		
		for i = 1 to n_submtl do
		(
			submtl = getSubmtl material i
			
			if submtl.diffuseMap != undefined then
			(
				diffuseMap = submtl.diffuseMap
				
				writeString file diffuseMap.filename
			)
			else
			(
				writeByte file 0
			)
		)
					
		for i = 1 to mesh.numverts do
		(
			vertex = getVert mesh i
			vertex = vertex * CM			
			
			normal = getNormal mesh i 
			normal = normal * CM	

			writeFloat file vertex.x
			writeFloat file vertex.y
			writeFloat file vertex.z
			
			writeFloat file normal.x
			writeFloat file normal.y
			writeFloat file normal.z			
		)
		
		for v = 1 to mesh.numtverts do
		(
			vert = getTVert mesh v
			
			writeFloat file vert.x
			writeFloat file vert.y
		)
	 
		for f = 1 to mesh.numfaces do
		(
			face = getFace mesh f
			
			writeLong file ( face.x - 1 )
			writeLong file ( face.y - 1 )
			writeLong file ( face.z - 1 )
				
			tvface = getTVFace mesh f
			
			writeLong file ( tvface.x - 1 )
			writeLong file ( tvface.y - 1 )
			writeLong file ( tvface.z - 1 )
			
			writeLong file ( getFaceMatID mesh f )
		)
		
	delete mesh	
	
	)
	
	fclose file
)
