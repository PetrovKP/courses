#version 130

varying vec3 origin, direction;

const int TYPE_DIFFUSE = 97;
const int TYPE_SHADOW = 98;
const int TYPE_REFLECTION = 96;
const int TYPE_TRANSPARENCY = 95;

const int TYPE_NONE = 0;
const int TYPE_SPHERE = 1;
const int TYPE_TRIANGLE = 2;

const int MAX_OBJECTS = 14;

const int TRIANGLE_NUMBER = 10;
const int SPHERE_NUMBER = 4;

const int MAX_MATERIALS_NUMBER = 11;
const int MAX_DEPTH = 2;
const int MAX_NODES = 4;

struct Material {
    vec3 ambient;           // фон
    vec3 diffuse;           // диффузная
    vec3 reflection;        // зеркальное отражение
    vec3 specular;          // зеркальные блики
    vec3 transparency;      // прозрачность
    vec3 emission;          // освещение
    vec3 atenuation;        // затухание
    float refractionCoef;   // коэф преломления
    float shiness;          // глянцевость
};

struct Sphere {
    vec3 position;
    float radius;
    int materialId;
};

struct Triangle {
    vec3 vertex1;
    vec3 vertex2;
    vec3 vertex3;
    int materialId;
};

struct Ray {
    vec3 origin;    // точка луча
    vec3 direction; // отронормированный вектор направления
    int type;
};

struct RayNode {
    Ray ray;
    vec3 reflectionColor;
    vec3 refractionColor;
    vec3 diffuseColor;
    vec3 specular;
    vec3 reflection;
    vec3 refraction;

    int parentIndex;
    int depth;
};

// Столкновение луча с объектом
struct HitInfo {
    bool hitDetected;   // Было ли столкновениие
    vec3 hitPoint;      // Точка столкновения
    vec3 surfaceNormal; // Вектор нормали в точке столкновения
    float distance;     // Расстояние от старта луча до точки столкновения
    int objectid;       // Номер объекта
    int objectType;     // Это тип объекта, если вы не поняли
};

RayNode rayNode[MAX_NODES];

Sphere spheres[SPHERE_NUMBER];
HitInfo sphereHitInfos[SPHERE_NUMBER];

Triangle triangles[TRIANGLE_NUMBER];
HitInfo triangleHitInfos[TRIANGLE_NUMBER];

Material materials[MAX_MATERIALS_NUMBER];

void initializeMaterials(){

    materials[0].diffuse = vec3(1);
    materials[0].ambient = vec3(1);
    materials[0].atenuation = vec3(0);
    materials[0].emission = vec3(1);
    materials[0].specular = vec3(1);
    materials[0].reflection = vec3(0);
    materials[0].transparency = vec3(0);
    materials[0].shiness = 0.0;
    materials[0].refractionCoef = 0.0;

    materials[1].diffuse = vec3(1.0, 0.4, 0.3);
    materials[1].ambient = vec3(1.0, 0.4, 0.3);
    materials[1].atenuation = vec3(0);
    materials[1].emission = vec3(0);
    materials[1].specular = vec3(0);
    materials[1].reflection = vec3(0);
    materials[1].transparency = vec3(0);
    materials[1].shiness = 0.0;
    materials[1].refractionCoef = 0.0;

    materials[2].diffuse = vec3(0.6, 0.6, 0.5);
    materials[2].ambient = vec3(0.6, 0.6, 0.5);
    materials[2].atenuation = vec3(0);
    materials[2].emission = vec3(0);
    materials[2].specular = vec3(0);
    materials[2].reflection = vec3(0);
    materials[2].transparency = vec3(0);
    materials[2].shiness = 0.0;
    materials[2].refractionCoef = 0.0;

    materials[3].diffuse = vec3(0.4, 0.1, 0.5);
    materials[3].ambient = vec3(0.4, 0.1, 0.5);
    materials[3].atenuation = vec3(0);
    materials[3].emission = vec3(0);
    materials[3].specular = vec3(0);
    materials[3].reflection = vec3(0);
    materials[3].transparency = vec3(0);
    materials[3].shiness = 0.0;
    materials[3].refractionCoef = 0.0;

    materials[4].diffuse = vec3(0.3, 1, 0.3);
    materials[4].ambient = vec3(0.3, 1, 0.3);
    materials[4].atenuation = vec3(0);
    materials[4].emission = vec3(0);
    materials[4].specular = vec3(0);
    materials[4].reflection = vec3(0);
    materials[4].transparency = vec3(0);
    materials[4].shiness = 0.0;
    materials[4].refractionCoef = 0.0;

    materials[5].diffuse = vec3(0.9, 0.3, 0.4);
    materials[5].ambient = vec3(0.9, 0.3, 0.4);
    materials[5].atenuation = vec3(0);
    materials[5].emission = vec3(0);
    materials[5].specular = vec3(0);
    materials[5].reflection = vec3(0);
    materials[5].transparency = vec3(0);
    materials[5].shiness = 0.0;
    materials[5].refractionCoef = 0.0;

    materials[6].diffuse = vec3(0.5, 0.3, 1.0);
    materials[6].ambient = vec3(0.5, 0.3, 1.0);
    materials[6].atenuation = vec3(0);
    materials[6].emission = vec3(0);
    materials[6].specular = vec3(0);
    materials[6].reflection = vec3(0);
    materials[6].transparency = vec3(0);
    materials[6].shiness = 0.0;
    materials[6].refractionCoef = 0.0;

    // Gold
    materials[7].diffuse = vec3(0.75164, 0.60648, 0.22648);
    materials[7].ambient = vec3(0.24725, 0.1995, 0.0745);
    materials[7].atenuation = vec3(1.0, 1.0, 6.0);
    materials[7].emission = vec3(0);
    materials[7].specular = vec3(1.0, 1.0, 1.0);
    materials[7].reflection = vec3(0.63, 0.56, 0.37);
//    material7[6].reflection =vec3(0);
    materials[7].transparency = vec3(0);
    materials[7].shiness = 50.0;
    materials[7].refractionCoef = 1.3;

    materials[8].diffuse = vec3(0.323, 0.34, 0.54);
    materials[8].ambient = vec3(0.42, 0.64, 0.75);
    materials[8].atenuation = vec3(0.4, 0.7, 0.0);
    materials[8].emission = vec3(0);
    materials[8].specular = vec3(1, 0.4, 0.5);
    materials[8].reflection = vec3(0);
    materials[8].transparency = vec3(0);
    materials[8].shiness = 40.0;
    materials[8].refractionCoef = 0.0;

    materials[9].diffuse = vec3(0.23, 0.86, 0.54);
    materials[9].ambient = vec3(0.65, 0.86, 0.42);
    materials[9].atenuation = vec3(1.0, 0.6, 0.8);
    materials[9].emission = vec3(0);
    materials[9].specular = vec3(0.4, 0.3, 0.6);
    materials[9].reflection = vec3(0);
    materials[9].transparency = vec3(0.8);
    materials[9].shiness = 0;
    materials[9].refractionCoef = 0.8;

    materials[10].diffuse = vec3(0.63, 0.26, 0.94);
    materials[10].ambient = vec3(0.65, 0.86, 0.32);
    materials[10].atenuation = vec3(0);
    materials[10].emission = vec3(0);
    materials[10].specular = vec3(0.4, 0.7, 0.2);
    materials[10].reflection = vec3(0);
    materials[10].transparency = vec3(0);
    materials[10].shiness = 20;
    materials[10].refractionCoef = 0.0;

}

void initializeObjects(){

    initializeMaterials();

// -------------------------------------------------------------------------
//                          ИНИЦИАЛИЗАЦИЯ СФЕР
// -------------------------------------------------------------------------

    spheres[0].position = vec3(-0.1, -0.3, -1.0);
    spheres[0].radius = 0.3;
    spheres[0].materialId = 7;

    spheres[1].position = vec3(-0.1, 0.2, 0.3);
    spheres[1].radius = 0.2;
    spheres[1].materialId = 0;

    spheres[2].position = vec3(0.7, 0.6, -3);
    spheres[2].radius = 0.35;
    spheres[2].materialId = 8;
//
//    spheres[3].position = vec3(0.3, 0.5, -1.7);
//    spheres[3].radius = 0.25;
//    spheres[3].material = materials[9];

    spheres[3].position = vec3(-0.3, 0.3, -1.0);
    spheres[3].radius = 0.2;
    spheres[3].materialId = 10;

// -------------------------------------------------------------------------
//                     ИНИЦИАЛИЗАЦИЯ ТРЕУГОЛЬНИКОВ(КОМНАТЫ)
// -------------------------------------------------------------------------

    triangles[0].vertex1 = vec3(-5.0, -5.0, 2.0);
    triangles[0].vertex2 = vec3(-5.0, -5.0, -10.0);
    triangles[0].vertex3 = vec3(-5.0, 5.0, 2.0);
    triangles[0].materialId = 1;

    triangles[1].vertex1 = vec3(-5.0, 5.0, 2.0);
    triangles[1].vertex2 = vec3(-5.0, -5.0, -10.0);
    triangles[1].vertex3 = vec3(-5.0, 5.0, -10.0);
    triangles[1].materialId = 1;

    triangles[2].vertex1 = vec3(-5.0, -5.0, 2.0);
    triangles[2].vertex2 = vec3(5.0, -5.0, 2.0);
    triangles[2].vertex3 = vec3(-5.0, -5.0, -10.0);
    triangles[2].materialId = 2;

    triangles[3].vertex1 = vec3(5.0, -5.0, 2.0);
    triangles[3].vertex2 = vec3(5.0, -5.0, -10.0);
    triangles[3].vertex3 = vec3(-5.0, -5.0, -10.0);
    triangles[3].materialId = 2;

    triangles[4].vertex1 = vec3(5.0, -5.0, 2.0);
    triangles[4].vertex2 = vec3(5.0, 5.0, 2.0);
    triangles[4].vertex3 = vec3(5.0, -5.0, -10.0);
    triangles[4].materialId = 3;

    triangles[5].vertex1 = vec3(5.0, 5.0, 2.0);
    triangles[5].vertex2 = vec3(5.0, 5.0, -10.0);
    triangles[5].vertex3 = vec3(5.0, -5.0, -10.0);
    triangles[5].materialId = 3;

    triangles[6].vertex1 = vec3(5.0, 5.0, 2.0);
    triangles[6].vertex2 = vec3(-5.0, 5.0, 2.0);
    triangles[6].vertex3 = vec3(5.0, 5.0, -10.0);
    triangles[6].materialId = 4;

    triangles[7].vertex1 = vec3(-5.0, 5.0, 2.0);
    triangles[7].vertex2 = vec3(-5.0, 5.0, -10.0);
    triangles[7].vertex3 = vec3(5.0, 5.0, -10.0);
    triangles[7].materialId = 4;

    triangles[8].vertex1 = vec3(-5.0, 5.0, -10.0);
    triangles[8].vertex2 = vec3(-5.0, -5.0, -10.0);
    triangles[8].vertex3 = vec3(5.0, 5.0, -10.0);
    triangles[8].materialId = 5;

    triangles[9].vertex1 = vec3(-5.0, -5.0, -10.0);
    triangles[9].vertex2 = vec3(5.0, -5.0, -10.0);
    triangles[9].vertex3 = vec3(5.0, 5.0, -10.0);
    triangles[9].materialId = 5;

//    triangles[10].vertex1 = vec3(-5.0, 5.0, 2.0);
//    triangles[10].vertex2 = vec3(5.0, -5.0, 2.0);
//    triangles[10].vertex3 = vec3(-5.0, -5.0, 2.0);
//    triangles[10].material = materials[6];
//
//    triangles[11].vertex1 = vec3(5.0, -5.0, 2.0);
//    triangles[11].vertex2 = vec3(-5.0, 5.0, 2.0);
//    triangles[11].vertex3 = vec3(5.0, 5.0, 2.0);
//    triangles[11].material = materials[6];

// -------------------------------------------------------------------------

}

// Вспомогательная функция для вычисления точки переечения
bool solveQuadratic(float a, float b, float c, inout float x0, inout float x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q;
        if (b > 0) {
            q = -0.5 * (b + sqrt(discr));
        } else {
            q =  -0.5 * (b - sqrt(discr));
        }
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) {
        float temp;
        temp = x0;
        x0=x1;
        x1=temp;
    }

    return true;
}

// Вычиления точки пересечения луча со сферой
bool sphereIntersect(Ray ray, int idSph, inout HitInfo hitinfo) {
    hitinfo.objectType = TYPE_NONE;

    float t0, t1; // solutions for t if the ray intersects
    float radius2 = spheres[idSph].radius*spheres[idSph].radius;

    vec3 L = ray.origin - spheres[idSph].position;
    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(L, ray.direction);
    float c = dot(L,L) - radius2;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    float temp;
    if (t0 > t1) {
        temp = t0;
        t0 = t1;
        t1 = temp;
    }

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) {
            hitinfo.hitDetected = false;
            return false; // both t0 and t1 are negative
        }
    }
    float t;
    t = t0;

    hitinfo.objectType = TYPE_SPHERE;
    hitinfo.hitPoint = ray.origin + ray.direction*t;
    hitinfo.surfaceNormal = normalize(hitinfo.hitPoint - spheres[idSph].position);
    hitinfo.objectid = idSph;
    hitinfo.distance = distance(hitinfo.hitPoint, ray.origin);
    hitinfo.hitDetected = true;
    return true;
}

// Пересечение с треугольником
void triangleInsert(Ray ray, int idTrian, inout HitInfo hitinfo) {
    hitinfo.objectType = TYPE_NONE;

    vec3 v1 = triangles[idTrian].vertex1;
    vec3 v2 = triangles[idTrian].vertex2;
    vec3 v3 = triangles[idTrian].vertex3;
    // compute plane's normal
    vec3 v1v2 = v2 - v1;

    vec3 v1v3 = v3 - v1;
    // no need to normalize
    vec3 N = cross(v1v2, v1v3); // N
    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = dot(N, ray.direction);
    if (abs(NdotRayDirection) < 1e-6) // almost 0
        return; // they are parallel so they don't intersect !
    // compute d parameter using equation 2

    float d = dot(N, v1);

    // compute t (equation 3)
    float t = -(dot(N, ray.origin) - d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) return; // the triangle is behind

    // compute the intersection point using equation 1
    vec3 P = ray.origin + t*ray.direction;

    // Step 2: inside-outside test
    vec3 C; // vector perpendicular to triangle's plane

    // edge 1
    vec3 edge1 = v2 - v1;
    vec3 vp1 = P - v1;
    C = cross(edge1, vp1);
    if (dot(N, C) < 0) return; // P is on the right side

    // edge 2
    vec3 edge2 = v3 - v2;
    vec3 vp2 = P - v2;
    C = cross(edge2, vp2);
    if (dot(N, C) < 0) return; // P is on the right side

    // edge 3
    vec3 edge3 = v1 - v3;
    vec3 vp3 = P - v3;
    C = cross(edge3, vp3);
    if (dot(N, C) < 0) return; // P is on the right side;

    hitinfo.objectType = TYPE_TRIANGLE;
    hitinfo.hitPoint = P;
    hitinfo.surfaceNormal = normalize(N);
    hitinfo.objectid = idTrian;
    hitinfo.distance = distance(P, ray.origin);
    hitinfo.hitDetected = true;
}

// Точка в тени?
bool isShadowed(vec3 hitPoint, int lightIndex, int lightType, inout vec3 transparency) {
    HitInfo hitInfoLight;
    hitInfoLight.hitDetected = false;
    Ray ray;
    if (lightType == TYPE_SPHERE) {
        Sphere light = spheres[lightIndex];
        vec3 eps = normalize(light.position - hitPoint) * 0.001;
        ray = Ray(hitPoint + eps, normalize(light.position - hitPoint), TYPE_SHADOW);
        sphereIntersect(ray, lightIndex, hitInfoLight);
    }
    else if (lightType == TYPE_TRIANGLE) {
        Triangle light = triangles[lightIndex];
        vec3 eps = normalize(light.vertex1 - hitPoint) * 0.001;
        ray = Ray(hitPoint + eps, normalize(light.vertex1 - hitPoint), TYPE_SHADOW);
        triangleInsert(ray, lightIndex, hitInfoLight);

    }

    float distance = hitInfoLight.distance; // Расстояние до источника света

    // Проверяем для каждого объекта, не загораживает ли он источник света
    HitInfo hitInfo;
    transparency = vec3(1.0);
    for (int i = 0; i < MAX_OBJECTS; i++) {
        hitInfo.hitDetected = false;
        Material material;
        int type;
        int index;
        if (i < SPHERE_NUMBER) {
            index = i;
            material = materials[spheres[index].materialId];
            type = TYPE_SPHERE;
            sphereIntersect(ray, index, hitInfo);
        }

        else {
            index = i - SPHERE_NUMBER;
            material = materials[triangles[index].materialId];
            type = TYPE_TRIANGLE;
            triangleInsert(ray, index, hitInfo);
        }
        if ((lightIndex != index || lightType != type) && hitInfo.hitDetected &&
             hitInfo.distance < distance) {
            // Если объект прозрачный - уменьшить прозрачность
            if (length(material.transparency) > 0) {
                transparency *= material.transparency;
                continue;
            }
            // Если объект непрозрачный - точка столкновения в тени от этого источнкика
            transparency = vec3(0.);
            return true;
        }
    }
    return false;
}


// Затенение по формуле Фонга
vec3 phogShading(Material material, Material lightMaterial, vec3 hitPoint, vec3 surfaceNormal,
                  vec3 lightDir, vec3 reflectDir, vec3 eyeDir, float distance) {
    // Расчёт затухания света от источника в зависимости от расстояния
   float attenuation = 1./(1. + lightMaterial.atenuation.x + distance*lightMaterial.atenuation.y +
                        distance*distance*lightMaterial.atenuation.z);

    // Расчёт коэффициентов в формуле Фонга
    float diffuseCoef = max(0., dot(surfaceNormal, lightDir));
    float scecularCoef = pow(dot(eyeDir, reflectDir), material.shiness);

    return material.ambient*lightMaterial.ambient +
           (material.diffuse*lightMaterial.emission*diffuseCoef +
           material.specular*lightMaterial.emission*scecularCoef)*attenuation;
}

vec3 calculateColor(HitInfo hitinfo) {
    Material material;

    if (hitinfo.objectType == TYPE_SPHERE)
        material = materials[spheres[hitinfo.objectid].materialId];
    else if (hitinfo.objectType == TYPE_TRIANGLE)
        material = materials[triangles[hitinfo.objectid].materialId];

    vec3 hitPoint = hitinfo.hitPoint;
    vec3 surfaceNormal = hitinfo.surfaceNormal;
    if (length(material.emission) > 0.0)
        return material.emission + material.diffuse;

    vec3 resultColor = vec3(0.0);
    for (int i = 0; i < MAX_OBJECTS; i++) {
        Material lightMaterial;
        vec3 lightPosition;
        int lightType;
        int lightIndex;
        if ( i < SPHERE_NUMBER ) {
            lightIndex = i;
            lightMaterial = materials[spheres[lightIndex].materialId];
            lightPosition = spheres[lightIndex].position;
            lightType = TYPE_SPHERE;
        }
        else {
            lightIndex = i - SPHERE_NUMBER;
            lightMaterial = materials[triangles[lightIndex].materialId];
            lightPosition = triangles[lightIndex].vertex1;
            lightType = TYPE_TRIANGLE;
        }

        vec3 transparency;
        if ((hitinfo.objectid != lightIndex || hitinfo.objectType != lightType) && length(lightMaterial.emission) > 0.0) {
            vec3 currentColor = vec3(0.0);
            if (!isShadowed(hitPoint, lightIndex, lightType, transparency)) {
                vec3 lightDir = lightPosition - hitPoint;
                float distance = length (lightDir);
                lightDir = normalize(lightDir);

                vec3 eyeDir = normalize(origin - hitPoint);
                vec3 reflectDir = vec3(0.0);
                if (dot(surfaceNormal, lightDir) > 0.0)
                    reflectDir = normalize(reflect(surfaceNormal, -lightDir));

                currentColor += phogShading(material, lightMaterial, hitPoint, surfaceNormal,
                                lightDir, reflectDir, eyeDir, distance)*transparency;
            }
            else {
                currentColor += lightMaterial.ambient * material.ambient;
            }
            resultColor += currentColor;
        }
    } // for
    return resultColor;
}

//--------------------------------------------------------------------------
// Нахождения пересечения луча со объектами
//--------------------------------------------------------------------------

vec3 iterativeRay(Ray ray) {

    initializeObjects();

    int numberOfNodes = 1;
    int currentNodeIndex = 0;

    rayNode[currentNodeIndex].ray = ray;
    rayNode[currentNodeIndex].depth = 0;

    float coeff;
    while (currentNodeIndex < numberOfNodes) {
        rayNode[currentNodeIndex].diffuseColor = vec3(0);
        rayNode[currentNodeIndex].reflectionColor = vec3(0);
        rayNode[currentNodeIndex].refractionColor = vec3(0);

        HitInfo hitinfo;
        hitinfo.hitDetected = false;
        hitinfo.objectType = TYPE_NONE;

        for (int i = 0; i < SPHERE_NUMBER; i++) {
            sphereIntersect(rayNode[currentNodeIndex].ray, i, sphereHitInfos[i]);
            if (sphereHitInfos[i].objectType != TYPE_NONE) {
                if (sphereHitInfos[i].distance < hitinfo.distance || hitinfo.objectType == TYPE_NONE) {
                    hitinfo = sphereHitInfos[i];
                }
            }
        }

        for (int i = 0; i < TRIANGLE_NUMBER; i++) {
            triangleInsert(rayNode[currentNodeIndex].ray, i, triangleHitInfos[i]);
            if (triangleHitInfos[i].objectType != TYPE_NONE) {
                if (triangleHitInfos[i].distance < hitinfo.distance || hitinfo.objectType == TYPE_NONE) {
                    hitinfo = triangleHitInfos[i];
                }
            }
        }

        if (hitinfo.hitDetected) {

            Material material;
            switch(hitinfo.objectType){
                case TYPE_SPHERE    : material = materials[spheres[hitinfo.objectid].materialId];    break;
                case TYPE_TRIANGLE  : material = materials[triangles[hitinfo.objectid].materialId];  break;
            }

            rayNode[currentNodeIndex].specular = material.specular;
            rayNode[currentNodeIndex].reflection = material.reflection;
            rayNode[currentNodeIndex].refraction = material.transparency;

////            // Генерация зеркального луча
            if ((length(material.reflection)>0) && (rayNode[currentNodeIndex].depth < MAX_DEPTH)) {

                vec3 reflectionDir = reflect(rayNode[currentNodeIndex].ray.direction, hitinfo.surfaceNormal);
//                vec3 offset = reflectionDir * 0.1;
                Ray newRay;
                newRay.origin = hitinfo.hitPoint * 0.1;
                newRay.direction = reflectionDir;
                newRay.type = TYPE_REFLECTION;
                rayNode[numberOfNodes].ray = newRay;
                rayNode[numberOfNodes].parentIndex = currentNodeIndex;
                rayNode[numberOfNodes].depth = rayNode[currentNodeIndex].depth + 1;
                numberOfNodes++;
            }

            // Генерация преломления луча
//            if ((rayNode[currentNodeIndex].depth < MAX_DEPTH) && (length(material.transparency) > 0) ) {
//                vec3 refractionDir = refract(rayNode[currentNodeIndex].ray.direction, hitinfo.surfaceNormal
//                                                            , material.refractionCoef);
////                vec3 offset = refractionDir * 0.01;
//                Ray newRay;
//                newRay.origin = hitinfo.hitPoint *0.01;
//                newRay.direction = refractionDir;
//                newRay.type = TYPE_TRANSPARENCY;
//                rayNode[numberOfNodes].ray = newRay;
//                rayNode[numberOfNodes].parentIndex = currentNodeIndex;
//                rayNode[numberOfNodes].depth = rayNode[currentNodeIndex].depth + 1;
//                numberOfNodes++;
//            }


            //  Считание влияния других источников света: диффузное рассеивание, блики
            if (length(material.ambient) > 0.0 || length(material.diffuse) > 0.0 ||
             length(material.specular) > 0.0 || rayNode[currentNodeIndex].depth >= MAX_DEPTH) {
                rayNode[currentNodeIndex].diffuseColor = calculateColor(hitinfo);
             }

        } else {
            // Если столкновения не было, задаём цвет фона
            rayNode[currentNodeIndex].diffuseColor = vec3(0.0);
            break;
        }
        currentNodeIndex++;
    } // while

    for(int i = currentNodeIndex - 1; i > 0; i--) {
        vec3 nodeColor = rayNode[i].diffuseColor + rayNode[i].reflectionColor*rayNode[i].reflection +
                         rayNode[i].refractionColor*rayNode[i].refraction;
        if (rayNode[i].ray.type == TYPE_REFLECTION )
            rayNode[rayNode[i].parentIndex].reflectionColor = nodeColor;
        else if ( rayNode[i].ray.type == TYPE_TRANSPARENCY )
            rayNode[rayNode[i].parentIndex].refractionColor = nodeColor;
    }

    return clamp(rayNode[0].diffuseColor + rayNode[0].reflectionColor*rayNode[0].reflection +
                rayNode[0].refractionColor*rayNode[0].refraction, vec3(0), vec3(1));
}

void main() {
    Ray ray = Ray(origin, direction, TYPE_DIFFUSE);
    gl_FragColor = vec4(iterativeRay(ray), 1);
}