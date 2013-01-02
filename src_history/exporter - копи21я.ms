out_name = getSaveFileName caption:"Export to O3D File" types:"O3D Files(*.o3d)|*.o3d" initialDir:(getDir #export) historyCategory:"ExportTextFiles"
	
struct D3DVertex
(
    pos    = [0,0,0],
    normal = [0,1,0],
    uv     = [0,0]
)

struct D3DFace
(
    indices = #(0,0,0)
)

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
		objVertices = #()
		objFaces    = #()
			
		pos = object.position
		rot = object.rotation
			
		mesh = object.mesh
	
		local zRot = rotateXMatrix -90
		
		pos = pos * zRot
		rot = rot * zRot
		
		local WorldMatrix = copy object.transform
			
		local CM = object.transform * ( inverse WorldMatrix )
		
		CM = CM * zRot 
		
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
		
		-- write diffuse texture name
		if object.material != undefined then
		(
			if object.material.diffusemap != undefined then
			(
				texname = filenamefrompath( object.material.diffusemap.filename )
				
				writestring file texname 
			)
		)
		else
		(
			writebyte file 0 -- no texture
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
				
			face = getTVFace mesh f
			
			writeLong file ( face.x - 1 )
			writeLong file ( face.y - 1 )
			writeLong file ( face.z - 1 )
		)
		
	delete mesh	
	
	)
	
	fclose file
)
