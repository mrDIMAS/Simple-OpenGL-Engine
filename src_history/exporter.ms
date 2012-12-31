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
		pos = object.position
		rot = object.rotation
			
		mesh = object.mesh
	
		local zRot = rotateXMatrix -90
		
		pos = pos * zRot
		rot = rot * zRot
		
		local WorldMatrix = copy object.transform
			
		local CM = object.transform * ( inverse WorldMatrix )
		
		CM = CM * zRot 
		
		num_faces  = mesh.numfaces			
				
		writeLong file mesh.faces.count 
		
		writeFloat file pos.x
		writeFloat file pos.y
		writeFloat file pos.z
		
		euler = quattoeuler( rot ) --order:6
				
		writeFloat file euler.x  
		writeFloat file euler.y
		writeFloat file euler.z	
		
		writeString file object.name
		
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
			writebyte file 0
		)
		
		for f = 1 to mesh.faces.count do
		(
		    local PosIndices  = getFace mesh f
		    local FaceNormals = meshop.getFaceRNormals mesh f
		    local UVIndices   = meshop.getMapFace mesh 1 f
		    
		    local FaceVert0 = D3DVertex()
		    local FaceVert1 = D3DVertex()
		    local FaceVert2 = D3DVertex()
		    
		    FaceVert0.pos = mesh.verts[ PosIndices.x ].pos
		    FaceVert1.pos = mesh.verts[ PosIndices.y ].pos
		    FaceVert2.pos = mesh.verts[ PosIndices.z ].pos
			
			FaceVert0.pos = FaceVert0.pos * CM
			FaceVert1.pos = FaceVert1.pos * CM
			FaceVert2.pos = FaceVert2.pos * CM			
		 			
		    FaceVert0.normal = FaceNormals[1]
		    FaceVert1.normal = FaceNormals[2]
		    FaceVert2.normal = FaceNormals[3]	

			
			FaceVert0.normal = FaceVert0.normal * CM
			FaceVert1.normal = FaceVert1.normal * CM
			FaceVert2.normal = FaceVert2.normal * CM			
			
		    FaceVert0.uv = [ (meshop.getMapVert mesh 1 UVIndices.x).x, (meshop.getMapVert mesh 1 UVIndices.x).y ]
		    FaceVert1.uv = [ (meshop.getMapVert mesh 1 UVIndices.y).x, (meshop.getMapVert mesh 1 UVIndices.y).y ]
		    FaceVert2.uv = [ (meshop.getMapVert mesh 1 UVIndices.z).x, (meshop.getMapVert mesh 1 UVIndices.z).y ]
		
			--vert 1
			writeFloat file FaceVert0.pos.x
			writeFloat file FaceVert0.pos.y
			writeFloat file FaceVert0.pos.z
			
			writeFloat file FaceVert0.normal.x
			writeFloat file FaceVert0.normal.y
			writeFloat file FaceVert0.normal.z
			
		 	writeFloat file FaceVert0.uv.x 
			writeFloat file FaceVert0.uv.y
			
			--vert 2
			writeFloat file FaceVert1.pos.x
			writeFloat file FaceVert1.pos.y
			writeFloat file FaceVert1.pos.z
			
			writeFloat file FaceVert1.normal.x
			writeFloat file FaceVert1.normal.y
			writeFloat file FaceVert1.normal.z
			
		 	writeFloat file FaceVert1.uv.x 
			writeFloat file FaceVert1.uv.y
		
			--vert 3
			writeFloat file FaceVert2.pos.x
			writeFloat file FaceVert2.pos.y
			writeFloat file FaceVert2.pos.z
		
			writeFloat file FaceVert2.normal.x
			writeFloat file FaceVert2.normal.y
			writeFloat file FaceVert2.normal.z
			
		 	writeFloat file FaceVert2.uv.x 
			writeFloat file FaceVert2.uv.y
			
		    local Face = D3DFace()
		
		    Face.indices[1] = ( f - 1 ) * 3
		    Face.indices[2] = Face.indices[1] + 1
		    Face.indices[3] = Face.indices[1] + 2
		
		   	writeLong file ( Face.indices[1] as Integer )
			writeLong file ( Face.indices[2] as Integer )
			writeLong file ( Face.indices[3] as Integer )

		)
		
		delete mesh
	)
	
	fclose file
)