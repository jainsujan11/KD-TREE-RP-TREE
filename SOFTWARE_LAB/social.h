/*
                DOCUMENTATION
1) PAIR --> Store X and Y Coordinates of Location
2) DATE --> Store Day, Month and Year
3) INDIVIDUAL --> Store all info related to particular individual
4) ORGANISATION --> Store all info related to particular organisation
5) BUSINESS --> Store all info related to particular business
6) GROUP --> Store all info related to particular group
7) 
*/
typedef struct 
{
    int x,y;
}pair;
typedef struct 
{
    int day,month,year;
}date;
typedef struct business business;
typedef struct organisation organisation;
typedef struct group group;
typedef struct individual individual;
struct individual
{
    int id;
    char name[100];
    char content[100];
    date creation_date;
    date birthday_date;
    business* bus_partner;
    group* gr_partner;
    organisation* org_partner;
};
struct business
{
    int id;
    char name[100];
    char content[100];
    date creation_date;
    pair location;
    individual* owners;
    individual* customers;
    group* gr_partner;
};
struct organisation
{
    int id;
    char name[100];
    char content[100];
    date creation_date;
    pair location;
    individual* members;
};
struct group
{
    int id;
    char name[100];
    char content[100];
    date creation_date;
    individual* ind_members;
    business* bus_members;
};
// ending structure definitions
individual* createIndividual()
{
    individual* temp = (individual*)malloc(sizeof(individual));
    // Fill the data
    return temp;
}
business* createBussiness()
{
    business* temp = (business*)malloc(sizeof(business));
    // Fill the data
    return temp;
}
group* createGroup()
{
    group* temp = (group*)malloc(sizeof(group));
    // Fill the data
    return temp;
}
organisation* createOrganisation()
{
    organisation* temp = (organisation*)malloc(sizeof(organisation));
    // Fill the data
    return temp;
}
void addOwnwer(business* B,individual* NEW)
{
    // B->owners = 
}
void addCustomer(business* B,individual* NEW)
{
    // B->owners = 
}
void add_Bussiness_to_Group(business* B,group* G)
{

}
void add_Individual_to_Group(individual* NEW,group* G)
{
    
}
void add_Individual_to_Organisation(individual* NEW,organisation* O)
{
    
}
void search_using_name()
{
    // search in individuals[]
    // search in business[]
    // search in org[]
    // search in grp[]
}
void search_using_type()
{
    //print all nodes of given type
}
void search_using_birthday()
{
    // birthdays is only of individuals
    // So search in individuals group
}
void one_hop_node_ind(individual* cur)
{
    // search in individual list for cur
}
void one_hop_node_group(group* cur)
{

}
void one_hop_node_org(organisation* cur)
{

}
void one_hop_node_business(business* cur)
{
    
}
void create_post_content()
{
    
}