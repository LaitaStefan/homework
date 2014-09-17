"""
    Papa Florin, 334CA
    Tema 1 ASC
"""

from Queue import Queue, Empty
from threading import Event, Thread
from barrier import *
from time import sleep, time


class Request():
    """
        class used to store request objects
    """

    def __init__(self, col, location, val, _type):
        self.col = col
        self.location = location
        self.val = val
        self._type = _type
        self.available = Event()
        self.available.clear()


class DataHandler(Thread):
    """
        Class that takes requests and passes them to other threads
    """

    def __init__(self, node):
        Thread.__init__(self)
        self.node = node
        self.finished = False
        self.workPool = Queue()
        self.threadList = []
        self.max_threads = 2

        thread_nr = self.node.datastore.get_max_pending_requests()
        
        if thread_nr > 0:
            self.max_threads = thread_nr - 3

        for i in range(self.max_threads):
            dataThread = DataThread(self)
            self.node.datastore.register_thread(self.node, dataThread)
            self.threadList.append(dataThread)
            dataThread.start()


    def declareFinished(self):
        self.finished = True


    def process_request(self, req):
        """
            process request from this node or other nodes
        """
        if req._type == "get":
            if req.location == "a":
                req.result = self.node.datastore.get_A(self.node, req.col)
            elif req.location == "b":
                req.result = self.node.datastore.get_b(self.node) 
        elif req._type == "put":
            if req.location == "a":
                self.node.datastore.put_A(self.node, req.col, req.val)
            elif req.location == "b":
                self.node.datastore.put_b(self.node, req.val)
        req.available.set()


    def getElementAt(self, n, location):
        """
            makes new get request and puts it in queue to be resolved
        """

        req = Request(n, location, "", "get")
        self.workPool.put(req)
        req.available.wait()
        return req.result


    def putElementAt(self, n, val, location):
        """
            makes new put request and puts it in queue to be resolved
        """

        req = Request(n, location, val, "put")
        self.workPool.put(req)


    def run(self):
        request = None

        sec_sleep = 0.0001

        if self.max_threads > 3:
            sec_sleep = 0.01

        while (not self.finished):
            try:
                request = self.workPool.get_nowait()
            except Empty:
                sleep(sec_sleep)
            else:
                self.process_request(request)


class DataThread(Thread):
    """
        Class that processes data requests
    """

    def __init__(self, dataHandler):
        Thread.__init__(self)
        self.dataHandler = dataHandler
        self.finished = False
        self.q = dataHandler.workPool
        self.node = self.dataHandler.node

    def declareFinished(self):
        self.finished = True


    def process_request(self, req):
        """
            process request from this node or other nodes
        """
        if req._type == "get":
            if req.location == "a":
                req.result = self.node.datastore.get_A(self.node, req.col)
            elif req.location == "b":
                req.result = self.node.datastore.get_b(self.node) 
        elif req._type == "put":
            if req.location == "a":
                self.node.datastore.put_A(self.node, req.col, req.val)
            elif req.location == "b":
                self.node.datastore.put_b(self.node, req.val)
        req.available.set()


    def run(self):
        """
            run function of the thread
        """
        request = None
        sec_sleep = 0.0001

        if self.dataHandler.max_threads > 3:
            sec_sleep = 0.01

        while (not self.finished):
            try:
                request = self.q.get_nowait()
            except Empty:
                sleep(sec_sleep)
            else:
                self.process_request(request)


class WorkerThread(Thread):
    """
        Class that computes the Gauss elimination
    """

    def __init__(self, node):
        Thread.__init__(self)
        self.node = node
        self.nodeList = node.nodes
        self.result = None        


    def my_max(self, k):
        """
            finds the index of the row with a non-zero element at position k
        """

        for i in range(k + 1, len(self.nodeList)):
            candidate = self.nodeList[i].getElementAt(k, "a");
            if candidate != 0:
                return i

        return k   


    def my_swap(self, k, j):
        """
            swaps two rows
        """

        if k != j:
            node_L = self.nodeList
            node_L[k], node_L[j] = node_L[j], node_L[k]
            node_L[k].set_id(k)
            node_L[j].set_id(j)


    def getValue(self, k):
        nodeL = self.nodeList
        current = nodeL[k].getElementAt(k, "a")

        if k == len(nodeL) - 1:
            res = nodeL[k].elem_kb / current
            nodeL[k].result = res
            nodeL[k].available.release()

        else:
            res = nodeL[k].elem_kb

            nodeL[k + 1].available.acquire()
            next_elem = nodeL[k].getElementAt(k + 1, "a")
            res -= next_elem * nodeL[k + 1].result
            nodeL[k + 1].available.release()

            for i in range(k + 2, len(nodeL)):
                res -= nodeL[k].getElementAt(i, "a") * nodeL[i].result

            res /= current
            nodeL[k].result = res
            nodeL[k].available.release()

        return res


    def gauss(self):
        """
            function that performs Gauss elimination
        """

        nodeL = self.nodeList

        for k in range(len(nodeL)):
            
            if nodeL[k].elem_kk == 0:

                i_max = self.my_max(k)

                self.my_swap(k, i_max)
                nodeL[k].elem_kk = nodeL[k].getElementAt(k, "a")
                nodeL[i_max].elem_kk = nodeL[i_max].getElementAt(i_max, "a")


            self.node.barrier.wait()

            if self.node.node_id > k:
                elem_ik = nodeL[self.node.node_id].getElementAt(k, "a")
                div = elem_ik / nodeL[k].elem_kk

                for j in range(k, len(nodeL)):
                    elem_ij = 0

                    if j == self.node.node_id:
                        elem_ij = nodeL[self.node.node_id].elem_kk
                    else:
                        elem_ij = nodeL[self.node.node_id].getElementAt(j, "a")

                    elem_kj = nodeL[k].getElementAt(j, "a")
                    elem_ij = elem_ij - elem_kj * div
                    nodeL[self.node.node_id].putElementAt(j, elem_ij, "a")

                    if j == self.node.node_id:
                        nodeL[self.node.node_id].elem_kk = elem_ij

                elem_ij = nodeL[self.node.node_id].elem_kb
                elem_kj = nodeL[k].elem_kb
                elem_ij = elem_ij - elem_kj * div
                nodeL[self.node.node_id].elem_kb = elem_ij

            self.node.barrier.wait()

        res = self.getValue(self.node.node_id)

        self.result = res, int(self.node.node_id)


    def run(self):
        """
            run method of worker thread
        """

        self.gauss()
