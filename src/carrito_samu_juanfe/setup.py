from setuptools import find_packages, setup

package_name = 'carrito_samu_juanfe'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='juanfe234qv',
    maintainer_email='juanfelipequiva@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
    'console_scripts':[
        'comando_comunicacion = carrito_samu_juanfe.comando_comunicacion:main',
        'comando_movimiento = carrito_samu_juanfe.comando_movimiento:main',
    ],
},
)
