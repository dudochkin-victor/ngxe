#!BPY
"""
Name: 'Simple exporter'
Blender: 234
Group: 'Export'
Tooltip: 'simple exporting script'
"""

import Blender
import sys
import struct
from Blender import Types,Object,NMesh


# file header
# struct { byte  id = 102
#          char name[6] = 'simple' 
#		   int  nrobjects} 
def saveheader(file):
	file.write(struct.pack("B6sL",102,'simple',len(Blender.Object.Get())));
	#make room for all index space
	fileindexlocation = file.tell()
	file.seek(file.tell() + 8*len(Blender.Object.Get())+4)
	return fileindexlocation





# index information
# struct { byte id;
#		   int32 offset }
def saveindex(file,id,fileindexlocation):
	pos = file.tell()
	file.seek(fileindexlocation)
	file.write(struct.pack("BL",id,pos))
	fileindexlocation = file.tell()
	file.seek(pos)
	return fileindexlocation

def savevertices(file,obj):
	print "saving vertex data..." 
	# vertice start structure
	# struct { byte id = 01; #00 means end of vertices
	#		   int32 vertices }	
	#
	vertices = obj.getData().verts
	#vertices start structure
	file.write(struct.pack("BL",01,vertices.__len__()))
	for v in vertices:
		#vertex data
		# struct { 	int32 index;
		#			float coordinate[3];
		#			float normal[3];
		#			float uvco[2] } 
		file.write(struct.pack("L",v.index))
		file.write(struct.pack("fff",v.co[0],v.co[1],v.co[2]))
		file.write(struct.pack("fff",v.no[0],v.no[1],v.no[2]))
		file.write(struct.pack("ff",v.uvco[0],v.uvco[1]))


def savefaces(file,obj):
	print "saving faces data..."	
	# faces start structre
	# struct { 	byte id = 02;
	#			int32 faces } 
	faces = obj.getData().faces;
	file.write(struct.pack("BL",02,faces.__len__()))
	for f in faces:
		# faces data:
		# struct { 
		# 	byte nrvertices;
		#	int32 vertices[4];
		#	float normal[3];
		#	float uv[8];
		#	}
		file.write(struct.pack("Bxxx",f.v.__len__()))
		ind = [0,0,0,0]
		i = 0
		for v in f.v:
			ind[i] = v.index
			i = i + 1
		for i in range(0,4):
			file.write(struct.pack("L",ind[i]))
		file.write(struct.pack("fff",f.no[0],f.no[1],f.no[2]))
		for i in range(0,len(f.uv)):
			file.write(struct.pack("ff",f.uv[i][0],f.uv[i][1]))
		for i in range(len(f.uv),4):
			file.write(struct.pack("ff",0,0))
		

def saveobjectinfo(file,obj):
	print "Saving object info..."
	#struct { 	byte id = 03
	#		  	float loc[3]
	#			float rot[3]
	#			float size[3]
	#			float matrixworld[16] }
	#  MORE WILL BE ADDED LATER
	file.write(struct.pack("Bxxx",03))		
	file.write(struct.pack("fff",obj.LocX,obj.LocY,obj.LocZ))
	file.write(struct.pack("fff",obj.RotX,obj.RotY,obj.RotZ))
	file.write(struct.pack("fff",obj.SizeX,obj.SizeY,obj.SizeZ))
	for i in obj.matrixWorld:
		for j in i:
			file.write(struct.pack("f",j))

def saveobject(file,obj):
	print "saving object... ",obj.getType(),file.tell()
	fileindexlocation = file.tell()
	file.seek(fileindexlocation+32)

	#03 = object global transformation data
	fileindexlocation = saveindex(file,03,fileindexlocation)
	saveobjectinfo(file,obj)	

	if type(obj.getData()) == Types.NMeshType:
		#01 = vertices
		fileindexlocation = saveindex(file,01,fileindexlocation)
		savevertices(file,obj)	

		#02 = faces
		fileindexlocation = saveindex(file,02,fileindexlocation)
		savefaces(file,obj)	

	#00 = no more!
	saveindex(file,00,fileindexlocation)	

		

def savefile(filename):
	file = open(filename,'wb')
	print "File created.. "
	#	fileindexlocation = saveheader(file)	
	objectindexlocation = saveheader(file)	
	
	objtype = dict()
	objtype["Camera"] = 01
	objtype["Mesh"] = 02
	objtype["Lamp"] = 03

	objects = Blender.Object.Get()
	for obj in objects:
		#write object index
		pos = file.tell()
		file.seek(objectindexlocation)		
		#object index:
		# struct { int type;
		# 	 	   int offset }
		file.write(struct.pack("LL",objtype[obj.getType()],pos))
		objectindexlocation = file.tell()
		file.seek(pos)
		saveobject(file,obj)	
		

	
	file.close()
	print "Done!"
	
Blender.Window.FileSelector(savefile,"Export to file","obj.simple")



#Copyright (C) 2005 Heinrich du Toit
#
#This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#version.
#
#This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
#details.
#
#You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
#The license can also be obtained electronically at www.fsf.org or www.freesoftware.org/licenses
#
#I can be electronically reached at the following email addresses:
#13806335@sun.ac.za (will work till end of 2005)
#nlhg@eject.co.za
