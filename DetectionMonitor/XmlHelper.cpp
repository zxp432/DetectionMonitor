//#pragma once
//namespace DetectionMonitor {
//	using namespace System;
//	using namespace System::Collections::Generic;
//	using namespace System;
//	using namespace System::Text;
//	using namespace System::Xml;
//	public ref class XmlHelper : Object {
//	public:static StringBuilder ^xpath = gcnew StringBuilder();
//		   static XmlDocument ^xmlDoc = gcnew XmlDocument();
//		   static XmlElement ^root = nullptr;
//		   static void AddCommint(String ^commit)
//		   {
//			   xpath->Append(commit);
//		   }
//		   static void AddNode(String ^name)
//		   {
//			   xpath->Append("/" + name);
//		   }
//		   static void AddNodeRegardlessPosition(String ^name)
//		   {
//			   xpath->Append("//" + name);
//		   }
//		   static void AddNodeByIndex(String ^name, String ^index)
//		   {
//			   xpath->Append("/" + name + "[" + index + "]");
//		   }
//		   static void AddNodeByIndexRegardlessPosition(String ^name, String ^index)
//		   {
//			   xpath->Append("//" + name + "[" + index + "]");
//		   }
//		   static void AddNodeByValue(String ^name, String ^value, String ^operatorstr = "=")
//		   {
//			   xpath->Append("[" + name + operatorstr + value + "]");
//		   }
//		   static void AddIndex(String ^index)
//		   {
//			   xpath->Append("[" + index + "]");
//		   }
//		   static void AddAttribute(String ^name)
//		   {
//			   xpath->Append("[@" + name + "]");
//		   }
//		   static void AddAttributeByValue(String ^name, String ^value, String ^operatorstr = "=")
//		   {
//			   xpath->Append("[@" + name + operatorstr + value + "]");
//		   }
//		   static void AddAnyNode()
//		   {
//			   xpath->Append("/*");
//		   }
//		   static void AddAnyNodeRegardlessPosition()
//		   {
//			   xpath->Append("//*");
//		   }
//		   static void AddAnyAttribute()
//		   {
//			   xpath->Append("[@*]");
//		   }
//		   static void Load(String ^filename)
//		   {
//			   xmlDoc->Load(filename);
//			   root = xmlDoc->DocumentElement;
//		   }
//
//		   static XmlNodeList ^Commit(String ^commit = nullptr)
//		   {
//			   XmlNodeList ^xn = nullptr;
//			   if (commit != nullptr)
//			   {
//				   xn = root->SelectNodes(commit);
//			   }
//
//			   else if (xpath->Length != 0)
//			   {
//				   xn = root->SelectNodes(XmlHelper::xpath->ToString());
//				   xpath->Clear();
//			   }
//
//			   return xn;
//		   }
//		   void addElement(String ^path, String ^node_root, String ^node_name, String ^node_text, String ^att_name, String ^att_value)
//		   {
//			   Load(path);
//
//			   XmlNodeList ^nodeList = xmlDoc->SelectSingleNode(node_root)->ChildNodes;//获取bookstore节点的所有子节点
//																					//判断是否有节点,有节点就遍历所有子节点,看看有没有重复节点,没节点就添加一个新节点
//			   if (nodeList->Count > 0)
//			   {
//				   for each(XmlNode ^xn in nodeList)//遍历所有子节点 
//				   {
//					   XmlElement ^xe = (XmlElement ^)xn;//将子节点类型转换为XmlElement类型 
//					   if (xe->GetAttribute(att_name) != att_value)
//					   {
//						   XmlNode ^xmldocSelect = xmlDoc->SelectSingleNode(node_root);   //选中根节点
//						   XmlElement ^son_node = xmlDoc->CreateElement(node_name);    //添加子节点 
//						   son_node->SetAttribute(att_name, att_value);     //设置属性
//						   son_node->InnerText = node_text;    //添加节点文本
//						   xmldocSelect->AppendChild(son_node);       //添加子节点
//						   xmlDoc->Save(path);          //保存xml文件
//						   break;
//					   }
//				   }
//
//			   }
//			   else
//			   {
//				   XmlNode ^xmldocSelect = xmlDoc->SelectSingleNode(node_root);   //选中根节点
//				   XmlElement ^son_node = xmlDoc->CreateElement(node_name);    //添加子节点 
//				   son_node->SetAttribute(att_name, att_value);     //设置属性
//				   son_node->InnerText = node_text;    //添加节点文本
//				   xmldocSelect->AppendChild(son_node);       //添加子节点
//				   xmlDoc->Save(path);          //保存xml文件
//			   }
//		   }
//
//		   /// <summary>
//		   /// 修改节点的内容
//		   /// </summary>
//		   /// <param name="path">xml文件的物理路径</param>
//		   /// <param name="node_root">根节点名称</param>
//		   /// <param name="new_text">节点的新内容</param>
//		   /// <param name="att_name">节点的属性名</param>
//		   /// <param name="att_value">节点的属性值</param>
//		   void UpdateElement(String ^path, String ^node_root, String ^new_text, String ^att_name, String ^att_value)
//		   {
//			   Load(path);
//			   XmlNodeList ^nodeList = xmlDoc->SelectSingleNode(node_root)->ChildNodes;//获取bookstore节点的所有子节点 
//			   for each(XmlNode ^xn in nodeList)//遍历所有子节点 
//			   {
//				   XmlElement ^xe = (XmlElement ^)xn;//将子节点类型转换为XmlElement类型 
//				   if (xe->GetAttribute(att_name) == att_value)
//				   {
//					   xe->InnerText = new_text;    //内容赋值
//					   xmlDoc->Save(path);//保存 
//					   break;
//				   }
//			   }
//
//		   }
//
//		   /// <summary>
//		   /// 删除节点
//		   /// </summary>
//		   /// <param name="path">xml文件的物理路径</param>
//		   /// <param name="node_root">根节点名称</param>
//		   /// <param name="att_name">节点的属性名</param>
//		   /// <param name="att_value">节点的属性值</param>
//		   void deleteNode(String ^path, String ^node_root, String ^att_name, String ^att_value)
//		   {
//			   Load(path);
//			   XmlNodeList ^nodeList = xmlDoc->SelectSingleNode(node_root)->ChildNodes;
//			   XmlNode ^root = xmlDoc->SelectSingleNode(node_root);
//			   for each(XmlNode ^xn in nodeList)
//			   {
//				   XmlElement ^xe = (XmlElement ^)xn;
//				   if (xe->GetAttribute(att_name) == att_value)
//				   {
//					   //xe->RemoveAttribute("name");//删除name属性 
//					   xe->RemoveAll();//删除该节点的全部内容 
//					   root->RemoveChild(xe);
//					   xmlDoc->Save(path);//保存 
//					   break;
//				   }
//
//			   }
//		   }
//
//	};
//}