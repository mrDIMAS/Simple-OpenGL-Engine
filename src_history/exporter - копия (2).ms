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
		
		local realMaterials = 0
		local subMaterials  = 0
		
		if( material != undefined ) then
		(
			subMaterials = getNumSubmtls material 	
			
			if( subMaterials == 0 ) then
				realMaterials = 1
			else
				realMaterials = subMaterials

			writeLong file realMaterials 
		)
		
		if( realMaterials > 0 ) then
		(
			if( subMaterials > 0 ) then
			(
				submtl = getSubmtl material i
					
				if submtl.diffuseMap != undefined then
				(
					diffuseMap = submtl.diffuseMap
					
					dfFile = filenamefrompath diffuseMap.filename
					
					writeString file dfFile 
				)
				else
				(
					writeByte file 0
				)
			)
			else
			(
				if material.diffuseMap != undefined then
				(
					dfFile = filenamefrompath material.diffuseMap.filename
					
					writeString file dfFile 
				)
				else
				(
					writeByte file 0
				)
			)
		)
		
		if( material != undefined ) then
		(
					
			
			if( n_submtl == 0 ) then
			(
				n_submtl = 1
				
				
				
				if material.diffuseMap != undefined then
				(
					dfFile = filenamefrompath material.diffuseMap.filename
					
					writeString file dfFile 
				)
				else
				(
					writeByte file 0
				)
			)
			else
			(
				writeLong file n_submtl
				
				for i = 1 to n_submtl do
				(
					submtl = getSubmtl material i
					
					if submtl.diffuseMap != undefined then
					(
						diffuseMap = submtl.diffuseMap
						
						dfFile = filenamefrompath diffuseMap.filename
						
						writeString file dfFile 
					)
					else
					(
						writeByte file 0
					)
				)
			)
		)
		else
		(
			writeLong file 1
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
			
			writeLong file (( getFaceMatID mesh f ) - 1)
		)
		
	delete mesh	
	
	)
	
	fclose file
)
