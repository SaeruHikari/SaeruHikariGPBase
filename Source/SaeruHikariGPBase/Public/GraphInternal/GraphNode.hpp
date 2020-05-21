#pragma once

template<typename T>
struct GraphEdge
{
protected:
	T AttachedData;
	TArray<TPair<int32, int32>> fromTo;
};

template<typename T>
struct GraphNode
{

protected:
	T AttachedData;
	TArray<GraphEdge> edges;
};
