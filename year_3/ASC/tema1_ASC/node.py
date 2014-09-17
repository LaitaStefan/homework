"""
    Papa Florin, 334CA

    This module represents a cluster's computational node.

    Computer Systems Architecture Course
    Assignment 1 - Cluster Activity Simulation
    March 2014
"""
from threads import *
from Queue import Queue
from threading import Event, Thread, Lock


class Node:
    """
        Class that represents a cluster node with computation and storage
        functionalities.
    """

    def __init__(self, node_id, matrix_size):
        """
            Constructor.

            @type node_id: Integer
            @param node_id: an integer less than 'matrix_size' uniquely
                identifying the node
            @type matrix_size: Integer
            @param matrix_size: the size of the matrix A
        """

        self.node_id = node_id
        self.matrix_size = matrix_size
        self.datastore = None
        self.nodes = None
        
        self.barrier = None
        self.worker = None
        self.available = Lock()
        self.available.acquire()
        self.result = None
        self.dataWorkers = []
        self.dataHandler = None
        self.elem_kk = 0
        self.elem_kb = 0


    def __str__(self):
        """
            Pretty prints this node.

            @rtype: String
            @return: a string containing this node's id
        """
        return "Node %d" % self.node_id


    def set_datastore(self, datastore):
        """
            Gives the node a reference to its datastore. Guaranteed to be called
            before the first call to 'get_x'.

            @type datastore: Datastore
            @param datastore: the datastore associated with this node
        """
        self.datastore = datastore

        self.dataHandler = DataHandler(self)
        self.datastore.register_thread(self, self.dataHandler)
        self.dataHandler.start()

        self.elem_kk = self.getElementAt(self.node_id, "a")
        self.elem_kb = self.getElementAt(self.node_id, "b")


    def set_nodes(self, nodes):
        """
            Informs the current node of the other nodes in the cluster. 
            Guaranteed to be called before the first call to 'get_x'.

            @type nodes: List of Node
            @param nodes: a list containing all the nodes in the cluster
        """
        self.nodes = nodes
        if self.node_id == 0 and self.barrier == None:
            barr = ReusableBarrierCond(len(self.nodes))
            for i in range(len(self.nodes)):
                self.nodes[i].setBarrier(barr)


    def get_x(self):
        """
            Computes the x value corresponding to this node. This method is
            invoked by the tester. This method must block until the result is
            available.

            @rtype: (Float, Integer)
            @return: the x value and the index of this variable in the solution
                vector
        """
        self.worker = WorkerThread(self)
        self.datastore.register_thread(self, self.worker)
        self.worker.start()
        self.worker.join()

        return self.worker.result


    def getElementAt(self, n, location):
        """
            Returns the element at the requested position in the current node's
            line
        """

        return self.dataHandler.getElementAt(n, location)
        


    def putElementAt(self, n, val, location):
        """
            Puts the specified value at the specified location in the current
            node's line
        """

        return self.dataHandler.putElementAt(n, val, location)


    def setBarrier(self, barr):
        self.barrier = barr


    def set_id(self, idx):
        self.node_id = idx


    def shutdown(self):
        """
            Instructs the node to shutdown (terminate all threads). This method
            is invoked by the tester. This method must block until all the
            threads started by this node terminate.
        """
        self.dataHandler.declareFinished()
        self.dataHandler.join()

        for i in self.dataHandler.threadList:
            i.declareFinished()
            i.join()
