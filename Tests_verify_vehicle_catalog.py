import csv, json, os
BASE=os.path.dirname(__file__)
path=os.path.join(BASE,'Content','Data','MetroverseVehicleCatalog.csv')
with open(path,encoding='utf-8',newline='') as f: rows=list(csv.DictReader(f))
assert len(rows)==140, f'Expected 140 entries, got {len(rows)}'
ids=[r['VehicleID'] for r in rows]
assert len(set(ids))==140, 'Vehicle IDs must be unique'
assert sum(r['Category']=='Car' for r in rows)==120
assert sum(r['Category']=='Bike' for r in rows)==20
required=['DisplayName','VehicleClass','Seats','TopSpeedKmh','BasePriceMC']
for r in rows:
    for k in required: assert r[k], f'Missing {k} in {r["VehicleID"]}'
    assert int(r['Seats'])>=1 and float(r['TopSpeedKmh'])>0 and int(r['BasePriceMC'])>0
print('PASS: 140 catalog entries; 120 cars; 20 bikes; unique IDs and required fields.')
