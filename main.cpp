
#include "mfem.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace mfem;

void meshmfem2afepack(mfem::Mesh *mesh, const char *meshfile)
{
   int dim, n_element, n_surface,n_side, n_node;

	dim = mesh->Dimension();
	std::cout << "DIM = " << dim << "\n";
	n_element = mesh->GetNE();// 网格中单元的数量
	n_surface = mesh->GetNumFaces();
	n_side    = mesh->GetNEdges();// 网格中边的数量
	std::cout << "  Num of Eleme = " << n_element
			  << "  Num of Faces = " << mesh->GetNFaces()
	          << "  Num of Edges = " << mesh->GetNEdges()
	          << "  Num of Point = " << mesh->GetNV()
	          << std::endl;
	n_node = mesh->GetNV();
	
	std::string realName(meshfile);
	realName = realName.substr(0, realName.rfind("."));
	std::ofstream os((realName + "-afepack.mesh").c_str());
	os.precision(8);
	os.setf(std::ios::scientific, std::ios::floatfield);

	//写入点坐标
	double *ep;
	Array<int> edgep;
    Array<int> cor;
	
	os << n_node << std::endl;
	for ( int i = 0; i < n_node; i++ )
	{
		ep = mesh->GetVertex(i);
		//mesh->GetNode(i,ep);
		if(dim == 2)
			os << ep[0] << "  " << ep[1] << std::endl;
		else
			os << ep[0] << "  " << ep[1] <<  "  " << ep[2] << std::endl;
	}	

	// 给每个边确定边界标识，afepack 要求给每个点也要给出边界标识
	// mfem 中没有直接给出，所以下面准备上述表示
	Array<int> VertexMark(n_node);
	VertexMark = 0;
	Array<int> faceMark(n_surface);
	faceMark = 0;
	Array<int> edgeMark(n_side);
	edgeMark = 0;
    for (int i = 0; i < mesh->GetNBE(); i++)
    {
     	int k = mesh->GetBdrElementEdgeIndex(i); //　2d返回边界单元边界边的全局边编号，3d返回边界单元边界面的全局面编号
    	//std::cout << i << " " << k << "  " << mesh->GetBdrAttribute(i) << std::endl;
    	if( dim == 3 )
    	{
			faceMark[k] = mesh->GetBdrAttribute(i);  // 取出第i个边界单元的属性赋值给边界面
			mesh->GetFaceEdges(k,edgep,cor);		 // 取出边界面的边
			for ( int j = 0; j < edgep.Size(); j++ )
				edgeMark[edgep[j]] = mesh->GetBdrAttribute(i);  // 给每个边界面的边赋值和面的属性相同

			mesh->GetFaceVertices(k,edgep);						// 取出边界面的顶点
			for ( int j = 0; j < edgep.Size(); j++ )
				VertexMark[edgep[j]] = mesh->GetBdrAttribute(i); 	// 给每个边界面的顶点赋值和面的属性相同
		}
		else
		{
			edgeMark[k] = mesh->GetBdrAttribute(i);		// 取出第i个边界单元的属性赋值给边界边
			mesh->GetEdgeVertices(k,edgep);						 // 取出边界边的顶点
			for ( int j = 0; j < edgep.Size(); j++ )
				VertexMark[edgep[j]] = mesh->GetBdrAttribute(i);
		}
	}

	//写入点编号
	os << std::endl<< n_node << std::endl;
	for ( int i = 0; i < n_node; i++ )
	{
		os << i << std::endl;
		os << 1 << "  " << i << std::endl;
		os << 1 << "  " << i << std::endl;
		os << VertexMark[i] << std::endl;
	}	
	
	//写入边的编号
	os << std::endl<< n_side << std::endl;
	for ( int i = 0; i < n_side; i++ )
	{
		mesh->GetEdgeVertices(i,edgep);
		os << i << std::endl;
		os << 2 << "  " << edgep[0] << " " << edgep[1] << std::endl;
		os << 2 << "  " << edgep[0] << " " << edgep[1] << std::endl;
		os << edgeMark[i] << std::endl;
	}	

	//写入面的编号
	if(dim == 3)
	{
		os << std::endl<< mesh->GetNFaces() << std::endl;
		for ( int i = 0; i < mesh->GetNFaces(); i++ )
		{
			mesh->GetFaceVertices(i,edgep);
			os << i << std::endl;
			os << edgep.Size();
			for( int j = 0; j < edgep.Size(); j++ )
				os << "  " << edgep[j];
			os << std::endl;
			mesh->GetFaceEdges(i,edgep,cor);
			os << edgep.Size();
			for( int j = 0; j < edgep.Size(); j++ )
				os << "  " << edgep[j];
			os << std::endl;
			os << faceMark[i] << std::endl;
		}	
	}
	//写入单元的编号
	os << std::endl<< n_element << std::endl;
	for ( int i = 0; i < n_element; i++ )
	{
		mesh->GetElementVertices(i,edgep);
		os << i << std::endl;
		os << edgep.Size();
		for( int j = 0; j < edgep.Size(); j++ )
			os << "  " << edgep[j];
		os << std::endl;
		if(dim == 3)
		{
			mesh->GetElementFaces(i,edgep,cor);
			os << edgep.Size();
			for( int j = 0; j < edgep.Size(); j++ )
				os << "  " << edgep[j];
			os << std::endl;
			//对于不分区域的网格，单元的属性暂时都定为　０
			//os << mesh->GetAttribute(i) << std::endl;
			os << 1 << std::endl;
		}
		else
		{
			mesh->GetElementEdges(i,edgep,cor);
			os << edgep.Size();
			for( int j = 0; j < edgep.Size(); j++ )
				os << "  " << edgep[j];
			os << std::endl;
			//对于不分区域的网格，单元的属性暂时都定为　０
			//os << mesh->GetAttribute(i) << std::endl;
			os << 1 << std::endl;
		}
	}	
	os.close();
   std::cout << "The mesh file : " << meshfile << " have been converted to the apfepack mesh file : " << (realName + "-afepack.mesh\n").c_str();
}

int main(int argc, char *argv[])
{
   // 1. Parse command-line options.
   const char *mesh_file = "../data/star.mesh";
   bool visualization = true;
   
   OptionsParser args(argc, argv);
   args.AddOption(&mesh_file, "-m", "--mesh",
                  "Mesh file to use.");
   args.AddOption(&visualization, "-vis", "--visualization", "-no-vis",
                  "--no-visualization",
                  "Enable or disable GLVis visualization.");
   args.Parse();
   if (!args.Good() || argc < 2)
   {
      args.PrintUsage(cout);
      return 1;
   }
   args.PrintOptions(cout);

   // 3. Read the mesh from the given mesh file. We can handle triangular,
   //    quadrilateral, tetrahedral, hexahedral, surface and volume meshes with
   //    the same code.
   mfem::Mesh mesh(mesh_file, 1, 1);
   mesh.Save("mfem.mesh");   
   meshmfem2afepack(&mesh, mesh_file);

   //mesh.Print();

   int dim = mesh.Dimension();
   int sdim = mesh.SpaceDimension();


   // 9. Connect to GLVis.
   char vishost[] = "localhost";
   int  visport   = 19916;
   socketstream sol_sock;
   if (visualization)
   {
      sol_sock.open(vishost, visport);
   }

   // 19. Send solution by socket to the GLVis server.
  if(visualization && sol_sock.good())
  {
     sol_sock.precision(8);
     sol_sock << "solution\n" << mesh << flush;
  }

   return 0;
}
