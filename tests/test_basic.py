import jwt
import uuid
import pytest
from testsuite.utils import matching

JWT_SECRET = '3zSPwp5OPzbRoI8iR7FjLvH6j8jDuSWxy3BZejcjrIL'

async def test_register_login(service_client):
    login = f"test_{uuid.uuid4().hex[:8]}"
    resp = await service_client.post('/api/auth/register', json={
        'login': login, 'password': 'secret123',
        'first_name': 'Test', 'last_name': 'User', 'email': f'{login}@test.com'
    })
    assert resp.status == 201
    user = resp.json()
    assert user['login'] == login

    resp = await service_client.post('/api/auth/login', json={'login': login, 'password': 'secret123'})
    assert resp.status == 200
    token = resp.json()['token']
    decoded = jwt.decode(token, JWT_SECRET, algorithms=['HS256'], audience='inventory-api')
    assert decoded['sub'] == user['id']

async def test_add_product(service_client):
    login = f"admin_{uuid.uuid4().hex[:6]}"
    await service_client.post('/api/auth/register', json={
        'login': login, 'password': 'pass1234', 'first_name': 'A', 'last_name': 'B',
        'email': f'{login}@t.com', 'role': 'admin'
    })
    login_resp = await service_client.post('/api/auth/login', json={'login': login, 'password': 'pass1234'})
    token = login_resp.json()['token']
    headers = {'Authorization': f'Bearer {token}'}
    resp = await service_client.post('/api/products', json={
        'name': 'Test product', 'initial_quantity': 10
    }, headers=headers)
    assert resp.status == 201
    assert resp.json()['name'] == 'Test product'