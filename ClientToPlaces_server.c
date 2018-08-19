/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "ClientToPlaces.h"
#include "PlacesToAirport.h"
#include "trie.h"
#include <string.h>
#include <iostream>
#include <locale>
#include <stdio.h>
using namespace std;


bool start = false;
trie* cityTrie;

void buildTree(){
    cityTrie = new trie();

    FILE *inFile;
    inFile = fopen(places2k, "r");
    char line[167];
    string state;
    string city;
    string place;
    double longitude;
    double latitude;
    
    while(fgets(line, 167, inFile) != NULL){
	string location(line);
        state = location.substr(0,2);
        city = location.substr(9, 63);
	
        place.append(state);
        place.append(city);
    
	
	locale loc;
	for(string::size_type i = 0; i < place.length(); i++){
	    place[i] = tolower(place[i],loc);
	}
        
	string lonStr = location.substr(153,7);
	string latStr = location.substr(144,5);

	longitude = stod(lonStr);
        latitude = stod(latStr);
        
	cityTrie->insert(place, longitude, latitude);
	place.clear();
    }
    
    fclose(inFile);
}


returnTypeC *
ctop_1_svc(city *argp, struct svc_req *rqstp)
{
     
    //build tree on first run 
    if(!start){
	buildTree();
	start = true;
    }
     

    static returnTypeC  result;

	/*
	 * insert server code here
	 */
    CLIENT* clnt;
    returnTypeA *result_1;
    LatLon ptoa_1_arg;
    char* host = "localhost";
    airports** airportList;
    places** listWrite;
    
    //free mem for consecutive calls
    xdr_free((xdrproc_t)xdr_returnTypeC, (char*)&result);
    
    //search tree here
    string searchStr;
    string stateStr(argp->state);
    string cityStr(argp->city);
    searchStr.append(stateStr);
    searchStr.append(cityStr);

    //force lowercase chars
    locale loc;
    for(string::size_type i = 0; i < searchStr.length(); i++){
        searchStr[i] = tolower(searchStr[i],loc);
    }
        
    struct node* place;
    place = cityTrie->search(searchStr); 

    //if place didn't find a location, give failed
    //message to the client
    if(place == NULL){
	listWrite = &result.returnTypeC_u.resultC;
	(*listWrite) = new places();
	strncpy((*listWrite)->name, "FAILED", 63);
	return &result;
    }
    else{
	ptoa_1_arg.lon = place->longitude;
	ptoa_1_arg.lat = place->latitude;
    }//if found fill args

#ifndef DEBUG
    clnt = clnt_create (host, PLACES_TO_AIRPORT, PLACES_TO_VERS, "udp");
    if (clnt == NULL) {
      clnt_pcreateerror (host);
      exit (1);
    }
#endif  /* DEBUG */
   

    result_1 = ptoa_1(&ptoa_1_arg, clnt);
    if (result_1 == (returnTypeA *) NULL) {
      clnt_perror (clnt, "call failed");
    }
    
    airportList = &result_1->returnTypeA_u.resultA;
    listWrite = &result.returnTypeC_u.resultC;
    
    //build the linked list that we send onward to the client
    //basic step logic for linked list, modified because the
    //head has the info on the user input city, meaning we 
    //overwrite that data if we see the head
    for(int i = 0; i < 6; i++){
	places* p = new places();
	strncpy(p->name, (*airportList)->name, 63);
	strncpy(p->code, (*airportList)->code, 3);
	strncpy(p->state, (*airportList)->state, 2);
	p->lat = (*airportList)->lat;
	p->lon = (*airportList)->lon;
	p->dist = (*airportList)->dist;
	places* temp = (*listWrite);
	if(temp == NULL){
	    strncpy(p->name, argp->city, 63);
	    strncpy(p->state, argp->state, 2);
	    p->lat = place->latitude;
	    p->lon = place->longitude;
	    (*listWrite) = p;
        }
    	else{
    	    while(temp->next){
    	    	temp = temp->next;
    	    }
    	    temp->next = p;
	}
	(*airportList) = (*airportList)->next;
    }
    

    clnt_freeres(clnt, (xdrproc_t) xdr_returnTypeA, (char *)&result_1);
    return &result;
}


