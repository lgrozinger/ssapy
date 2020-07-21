import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="ssapy",
    version="0.0.1",
    author="Lewis Grozinger",
    author_email="l.grozinger2@ncl.ac.uk",
    description="A package wrapped a C stochastic simulation library.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/lgrozinger/ssapy.git",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)
