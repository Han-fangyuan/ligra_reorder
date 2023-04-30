
order_identity(n)
order_rand
————————————————————
order_deg
order_rcm
order_gorder []
————————————————————————
order_ldg 
order_hubsort
order_selective_hubsort
order_hubcluster
order_dbg
order_minla
order_minloga
order_slashburn []




		
		if(order_name == "deg" or order_name == "degDesc") rank = order_deg(h, /*desc=*/true);
		else if(order_name == "rcm") rank = order_rcm(h,directed);
		else if(order_name == "gorder") 
		{
		  number = (number == 0) ? 5 : number;
		  rank = complete_gorder(h, number,directed);
		 
		}
		//minla,minloga,slashburn,ldg重排序，
		else 
		{
			Info("转换到adjacency list");
			Adjlist* g;
			if(directed) g = new Dadjlist(h);
			else g = new Uadjlist(h);
			TimeStep("Adjlist")

			if(order_name == "ldg") rank = order_ldg(*g);
			else if(order_name == "hubsort")  rank = order_hubsort(*g);
			else if(order_name == "selective_hubsort")  rank = order_selective_hubsort(*g);
			else if(order_name == "hubclustet")  rank = order_hubcluster(*g);
			else if(order_name == "dbg")  rank = order_dbg(*g);
      		else if(order_name == "minla")  rank = order_minla(*g);
      		else if(order_name == "minloga")  rank = order_minloga(*g);
      		else if(order_name == "slashburn")  rank = order_slashburn(*g);
			else { Alert("Unknown order `" << order_name <<"`"); return 1; }
      
   	    }
