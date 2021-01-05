from pathlib import Path
from setuptools import setup, find_packages

ROOT = Path(__file__).parent

setup(
    name='momos',
    version='0.0.1',
    description='Tool for generating state machines tests',
    author='Christoph Swoboda',
    author_email='codetent@gmail.com',
    license='MIT',
    packages=find_packages(exclude=['examples', 'doc']),
    install_requires=[
        'comment_parser==1.2.3',
        'Jinja2==2.11.2',
        'lark_parser==0.11.1',
        'click==7.1.2',
        'networkx==2.5',
        'case_converter==1.0.2',
        'lark==0.11.1',
    ],
    entry_points=dict(console_scripts=['momos=momos.cli:run']),
    platforms=['POSIX', 'Windows'],
    zip_safe=False,
    long_description=(ROOT / 'README.md').read_text(),
    package_data={
        'momos': [
            'parser/grammar.lark',
            'libraries/**/*',
            'templates/**/*',
        ],
    },
)
