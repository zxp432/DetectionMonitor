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
//			   XmlNodeList ^nodeList = xmlDoc->SelectSingleNode(node_root)->ChildNodes;//��ȡbookstore�ڵ�������ӽڵ�
//																					//�ж��Ƿ��нڵ�,�нڵ�ͱ��������ӽڵ�,������û���ظ��ڵ�,û�ڵ�����һ���½ڵ�
//			   if (nodeList->Count > 0)
//			   {
//				   for each(XmlNode ^xn in nodeList)//���������ӽڵ� 
//				   {
//					   XmlElement ^xe = (XmlElement ^)xn;//���ӽڵ�����ת��ΪXmlElement���� 
//					   if (xe->GetAttribute(att_name) != att_value)
//					   {
//						   XmlNode ^xmldocSelect = xmlDoc->SelectSingleNode(node_root);   //ѡ�и��ڵ�
//						   XmlElement ^son_node = xmlDoc->CreateElement(node_name);    //����ӽڵ� 
//						   son_node->SetAttribute(att_name, att_value);     //��������
//						   son_node->InnerText = node_text;    //��ӽڵ��ı�
//						   xmldocSelect->AppendChild(son_node);       //����ӽڵ�
//						   xmlDoc->Save(path);          //����xml�ļ�
//						   break;
//					   }
//				   }
//
//			   }
//			   else
//			   {
//				   XmlNode ^xmldocSelect = xmlDoc->SelectSingleNode(node_root);   //ѡ�и��ڵ�
//				   XmlElement ^son_node = xmlDoc->CreateElement(node_name);    //����ӽڵ� 
//				   son_node->SetAttribute(att_name, att_value);     //��������
//				   son_node->InnerText = node_text;    //��ӽڵ��ı�
//				   xmldocSelect->AppendChild(son_node);       //����ӽڵ�
//				   xmlDoc->Save(path);          //����xml�ļ�
//			   }
//		   }
//
//		   /// <summary>
//		   /// �޸Ľڵ������
//		   /// </summary>
//		   /// <param name="path">xml�ļ�������·��</param>
//		   /// <param name="node_root">���ڵ�����</param>
//		   /// <param name="new_text">�ڵ��������</param>
//		   /// <param name="att_name">�ڵ��������</param>
//		   /// <param name="att_value">�ڵ������ֵ</param>
//		   void UpdateElement(String ^path, String ^node_root, String ^new_text, String ^att_name, String ^att_value)
//		   {
//			   Load(path);
//			   XmlNodeList ^nodeList = xmlDoc->SelectSingleNode(node_root)->ChildNodes;//��ȡbookstore�ڵ�������ӽڵ� 
//			   for each(XmlNode ^xn in nodeList)//���������ӽڵ� 
//			   {
//				   XmlElement ^xe = (XmlElement ^)xn;//���ӽڵ�����ת��ΪXmlElement���� 
//				   if (xe->GetAttribute(att_name) == att_value)
//				   {
//					   xe->InnerText = new_text;    //���ݸ�ֵ
//					   xmlDoc->Save(path);//���� 
//					   break;
//				   }
//			   }
//
//		   }
//
//		   /// <summary>
//		   /// ɾ���ڵ�
//		   /// </summary>
//		   /// <param name="path">xml�ļ�������·��</param>
//		   /// <param name="node_root">���ڵ�����</param>
//		   /// <param name="att_name">�ڵ��������</param>
//		   /// <param name="att_value">�ڵ������ֵ</param>
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
//					   //xe->RemoveAttribute("name");//ɾ��name���� 
//					   xe->RemoveAll();//ɾ���ýڵ��ȫ������ 
//					   root->RemoveChild(xe);
//					   xmlDoc->Save(path);//���� 
//					   break;
//				   }
//
//			   }
//		   }
//
//	};
//}