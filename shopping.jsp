<%@ page language="java" pageEncoding="GBK"%>
<html>
<head>
<title>简易购物车</title>
</head>
<body >
<%
	int number=0,flag=0,kind=0,a=0,j=0;
	String temp=null,now=null,temp2=null;
	String[] goods=new String[100];
	int[] amount=new int[100];
	
	if(null!=session.getAttribute("number")){
		number=(Integer)session.getAttribute("number");		
		kind=(Integer)session.getAttribute("kind");	
	}
	if(request.getParameter("wuping")!="" ){
		now=request.getParameter("wuping");
		if(request.getParameter("sub3")!=null){
			for(int i=1;i<=kind;i++){
				temp=(String)session.getAttribute("goods["+i+"]");
				if(now.equals(temp)){
					number=number-(Integer)session.getAttribute("amount["+i+"]");
					session.setAttribute("amount["+i+"]",0);
					j=i;
					flag=1;break;
				}
				
			}
			if(flag==1){
				if(j==kind){
					kind--;
				}
				else {
					for(int i=j;i<kind;i++){
						j=i+1;
						temp2=(String)session.getAttribute("goods["+j+"]");
						a=(Integer)session.getAttribute("amount["+j+"]");
						session.setAttribute("amount["+i+"]",a);
						session.setAttribute("goods["+i+"]",temp2);
					}
					session.setAttribute("amount["+kind+"]",0);
					kind--;
					
				}
			}
			
		}
		else{
			for(int i=1;i<=kind;i++){
				temp=(String)session.getAttribute("goods["+i+"]");
				if(now.equals(temp)){
						session.setAttribute("amount["+i+"]",(Integer)session.getAttribute("amount["+i+"]")+1);
						flag=1;break;
				}
			}
		}
		if(flag==0 && request.getParameter("sub3")==null){
			kind++;
			session.setAttribute("amount["+kind+"]",1);
			goods[number]=now;
			session.setAttribute("goods["+kind+"]",now);
		}
		if(request.getParameter("sub3")==null)
			number++;
		session.setAttribute("number",number);	
		session.setAttribute("kind",kind);
	}	
	number--;
	kind--;
	out.println("当前物品有"+number+"件"+"<br>");
	out.println("当前物品种类有"+kind+"件"+"<br>"+"<br>");
	out.println("物品 数量"+"<br>");
	for(int i=2;i<=kind+1;i++){				
		out.println(session.getAttribute("goods["+i+"]")+"&nbsp&nbsp");
		out.println(session.getAttribute("amount["+i+"]")+"<br>");
	}
	
	
%>
<%
	if(request.getParameter("sub2")!=null){
		session.invalidate();	
		response.sendRedirect("shopping.jsp");
	}
%>


<form action="" method="post">
<table >
<tr>
<td> <input name="wuping" type="text"></td>
<td> 输入要买的商品名称 </td>
</tr>
<tr><td><input name="sub1" type="submit" value="加入购物车" style="background-image:url('1.gif');width: 125; height: 30;border: blue; cursor: pointer;" ></td>
<td><input name="sub2" type="submit" value="清空购物车" ></td>
<td><input name="sub3" type="submit" value="清空指定物品" ></td></tr>
</table>
</form>
</body>
</html>