import multiprocessing as mp
import os
import sys
import _posixshmem
import subprocess
import time

def get_start_method():
    if len(sys.argv) == 1:
        return 'spawn'
    if len(sys.argv) > 2 or sys.argv[1] not in ['spawn', 'fork', 'forkserver']:
        print(f"Incorrect usage! Usage: {os.getcwd()}/mp_test.py [fork|spawn|forkserver]")
        sys.exit(1)
    return sys.argv[1]
    
def hello(name):
    print('hello', name)

def test_mp_Process_basic():
    p = mp.Process(target=hello, args=('bob',))
    p.start()
    p.join()

def _resource_unlink(name, rtype):
    assert(rtype == "shared_memory")
    _posixshmem.shm_unlink(name)

def test_resource_tracker():
    #
    # Check that killing process does not leak named semaphores
    #
    cmd = '''if 1:
        import time, os, tempfile
        import multiprocessing as mp
        from multiprocessing.shared_memory import SharedMemory

        mp.set_start_method("spawn")
        rand = tempfile._RandomNameSequence()


        def create_and_register_resource(rtype):
            if rtype == "semaphore":
                lock = mp.Lock()
                return lock, lock._semlock.name
            elif rtype == "shared_memory":
                sm = SharedMemory(create=True, size=10)
                return sm, sm._name
            else:
                raise ValueError(
                    "Resource type {{}} not understood".format(rtype))


        resource1, rname1 = create_and_register_resource("{rtype}")
        resource2, rname2 = create_and_register_resource("{rtype}")

        os.write({w}, rname1.encode("ascii") + b"\\n")
        os.write({w}, rname2.encode("ascii") + b"\\n")

        time.sleep(10)
    '''
    for rtype in ['shared_memory']:
        # if rtype == "noop" or rtype == "semaphore":
        #     # Artefact resource type used by the resource_tracker
        #     continue
        r, w = os.pipe()
        p = subprocess.Popen([sys.executable,
                                '-E', '-c', cmd.format(w=w, rtype=rtype)],
                                pass_fds=[w],
                                stderr=subprocess.PIPE)
        os.close(w)
        with open(r, 'rb', closefd=True) as f:
            name1 = f.readline().rstrip().decode('ascii')
            name2 = f.readline().rstrip().decode('ascii')
        _resource_unlink(name1, rtype)
        time.sleep(2)
        p.terminate()
        p.wait()

        deadline = time.monotonic() + 5 * 60.0
        while time.monotonic() < deadline:
            time.sleep(2)
            try:
                _resource_unlink(name2, rtype)
            except OSError as e:
                # docs say it should be ENOENT, but OSX seems to give
                # EINVAL
                print(f"errno={e.errno}")
                break
        else:
            raise AssertionError(
                f"A {rtype} resource was leaked after a process was "
                f"abruptly terminated.")
        err = p.stderr.read().decode('utf-8')
        p.stderr.close()
        expected = ('resource_tracker: There appear to be 2 leaked {} '
                    'objects'.format(
                    rtype))
        print("===============")
        print("err:")
        print(err)
        print("===============")
        print("expected:")
        print(expected)


# def double(x):
#     return 2*x

# def test_mp_Pool_basic():
#     with mp.Pool(5) as p:
#         print(p.map(double, [1, 2, 3]))

if __name__ == '__main__':
    mp.set_start_method(get_start_method())
    start_method = mp.get_start_method()
    print(f'python version:{sys.version} \ndefault start_method={start_method}')
    #test_mp_Process_basic()
    test_resource_tracker()

