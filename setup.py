from setuptools import setup
from setuptools.command.install import install
import subprocess


with open("README.md", "r") as fh:
    long_description = fh.read()



class InstallWithSO(install):
    """Install command which builds libssa"""
    def run(self):
        subprocess.check_call('make build', shell=True)
        super().run()


setup(
    name="ssapy",
    version="0.0.1",
    author="Lewis Grozinger",
    author_email="l.grozinger2@ncl.ac.uk",
    description="A package wrapping a C stochastic simulation library.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/lgrozinger/ssapy.git",
    packages=['ssapy'],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
    cmdclass={'install': InstallWithSO}

