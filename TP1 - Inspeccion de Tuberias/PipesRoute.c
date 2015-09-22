//
//  PipesRoute.c
//  TP1 - Inspeccion de Tuberias
//
//  Created by Gastón Montes on 9/9/15.
//  Copyright (c) 2015 Gastón Montes. All rights reserved.
//

#include <stdlib.h>

#include "PipesRoute.h"

static int const kNodeNameMaxLenght = 3; // name + \n char.

void createList(PipesRoute *route, File *pipesFile, File *routeFiles) {
	char *nodeNameBuffer = (char *)malloc(sizeof(char) * kNodeNameMaxLenght);

	fileReadLine(routeFiles, nodeNameBuffer, kNodeNameMaxLenght);

	while (fileEndOfFile(routeFiles) != EOF) {
		struct RouteNode *newNode = routeNodeCreate(nodeNameBuffer[0]);
		pipesRouteAddNode(route, newNode);
		fileReadLine(routeFiles, nodeNameBuffer, kNodeNameMaxLenght);
	}

	free(nodeNameBuffer);
}

void pipesRouteCreate(PipesRoute **route, File *pipesFile, File *routeFiles) {
	PipesRoute *newRoute = (PipesRoute *)malloc(sizeof(PipesRoute));
	newRoute->headNode = NULL;
	*route = newRoute;

	createList(*route, pipesFile, routeFiles);
}

void pipesRouteDestroy(PipesRoute *route) {
	struct RouteNode *currentNode = route->headNode;
	struct RouteNode *nextNode = NULL;

	while (currentNode != NULL) {
		nextNode = routeNodeNext(currentNode);

		routeNodeDestroy(currentNode);

		currentNode = nextNode;
	}

	free(route);
}

void pipesRouteAddNode(PipesRoute *route, struct RouteNode *node) {
	struct RouteNode *currentNode = route->headNode;

	if (currentNode == NULL) {
		printf("Adding head node: %c\n", node->nodeName);
		route->headNode = node;
		routeNodeSetDistanceToRoot(node, 0);
	} else {
		struct RouteNode *prevNode = NULL;

		while (currentNode != NULL) {
			prevNode = currentNode;
			currentNode = routeNodeNext(currentNode);
		}
		routeNodeSetNext(prevNode, node);

		int prevDistanceToRoot = routeNodeDistanceToRoot(prevNode);
		int distanceToPrev = 2; // Calculate distance to previous.
		routeNodeSetDistanceToRoot(node, prevDistanceToRoot + distanceToPrev);

		printf("Adding node: %c.\n", node->nodeName);
		printf("Previous node: %c.\n", prevNode->nodeName);
		printf("Distance to root: %i.\n", node->distanceToRoot);
	}
}
